/*
* @File: region_param.cpp
* @Author: tangxuan
*
* @Created on January 1, 2017, 5:11 PM
*/

#include "region_param.h"
#include "segment_image.h"
#include "pnmfile.h"
#include "anigauss.h"
#include "filter.h"
#include "segment_graph.h"

#define INFIN 1.0e10
#define NEG_INFIN -(INFIN)

int *segment_image_index(image<rgb> *im, float sigma, float c, int min_size, int *num_ccs)
{
    int i_width = im->width();
	int i_height = im->height();

	image<float> *fl_r = new image<float>(i_width, i_height);
	image<float> *fl_g = new image<float>(i_width, i_height);
	image<float> *fl_b = new image<float>(i_width, i_height);

    // smooth each color channel
	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {
			imRef(fl_r, x, y) = imRef(im, x, y).r;
			imRef(fl_g, x, y) = imRef(im, x, y).g;
			imRef(fl_b, x, y) = imRef(im, x, y).b;
        }
    }

	image<float> *pofl_smooth_r = smooth(fl_r, sigma);
	image<float> *pofl_smooth_g = smooth(fl_g, sigma);
	image<float> *pofl_smooth_b = smooth(fl_b, sigma);

	delete fl_r;
	delete fl_g;
	delete fl_b;

    // build graph
	edge *po_edges = new edge[i_width * i_height * 4];
    int n_num = 0;

	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {

			if (x < i_width - 1) {
				po_edges[n_num].a = y * i_width + x;
				po_edges[n_num].b = y * i_width + (x + 1);
				po_edges[n_num].w = diff(pofl_smooth_r, pofl_smooth_g, pofl_smooth_b, x, y, x + 1, y);
				n_num++;
            }

			if (y < i_height - 1) {
				po_edges[n_num].a = y * i_width + x;
				po_edges[n_num].b = (y + 1) * i_width + x;
				po_edges[n_num].w = diff(pofl_smooth_r, pofl_smooth_g, pofl_smooth_b, x, y, x, y + 1);
				n_num++;
            }

			if ((x < i_width - 1) && (y < i_height - 1)) {
				po_edges[n_num].a = y * i_width + x;
				po_edges[n_num].b = (y + 1) * i_width + (x + 1);
				po_edges[n_num].w = diff(pofl_smooth_r, pofl_smooth_g, pofl_smooth_b, x, y, x + 1, y + 1);
				n_num++;
            }

			if ((x < i_width - 1) && (y > 0)) {
				po_edges[n_num].a = y * i_width + x;
				po_edges[n_num].b = (y - 1) * i_width + (x + 1);
				po_edges[n_num].w = diff(pofl_smooth_r, pofl_smooth_g, pofl_smooth_b, x, y, x + 1, y - 1);
				n_num++;
            }
        }
    }
	delete pofl_smooth_r;
	delete pofl_smooth_g;
	delete pofl_smooth_b;

    // segment
	Universe *po_uni = segment_graph(i_width * i_height, n_num, po_edges, c);

    // post process small components
	for (int i = 0; i < n_num; i++) {
		int i_a = po_uni->find(po_edges[i].a);
		int i_b = po_uni->find(po_edges[i].b);
		if ((i_a != i_b) && ((po_uni->size(i_a) < min_size) || (po_uni->size(i_b) < min_size)))
			po_uni->join(i_a, i_b);
    }

    delete[] po_edges;
	*num_ccs = po_uni->num_sets();

    // assign the each region with an index color
	int *pai_color_index = new int[i_width * i_height];

	for (int i = 0; i < i_width * i_height; i++) {
		pai_color_index[i] = 0;
	}

    int n_idx = 1;
	int *pai_indexmap = new int[i_width * i_height];   //checkout if indexmap is delete or not 

	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {
			int comp = po_uni->find(y * i_width + x);

			if (!(pai_color_index[comp])) {
				pai_color_index[comp] = n_idx;
				n_idx += 1;
            }

			pai_indexmap[y * i_width + x] = pai_color_index[comp];
        }
    }

	delete[] pai_color_index;
	delete po_uni;

	return pai_indexmap;
}

//TODO:

int *track_region_neighbors(int *indexmap, int height, int width, int num_css) /* check there  2017.01.07 20:30 */ 
{
	int *pai_neighbors = new int[num_css * num_css];

    for (int i = 0; i < num_css * num_css; i++) {
		pai_neighbors[i] = 0;
    }

	int i_vcurr, i_hcurr, i_curr;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
			i_curr = indexmap[y * width + x]; // current pixel's color index

            if (x > 0) {
				i_hcurr = indexmap[y * width + x - 1]; // previous horizontal pixel's color index
				pai_neighbors[(i_hcurr - 1) * num_css + i_curr - 1] = 1;
				pai_neighbors[(i_curr - 1) * num_css + i_hcurr - 1] = 1;
            }

            if (y > 0) {
				i_vcurr = indexmap[(y - 1) * width + x]; // previous vertical pixel's color index
				pai_neighbors[(i_vcurr - 1) * num_css + i_curr - 1] = 1;
				pai_neighbors[(i_curr - 1) * num_css + i_vcurr - 1] = 1;
            }
        }
    }

    for (int i = 0; i < num_css; i++) {
		pai_neighbors[i * num_css + i] = 0;
    }

	return pai_neighbors;
}

int *get_region_bound(int *indexmap, int height, int width, int num_css)
{
    int *pai_bound = new int[num_css * 4];

    for (int i = 0; i < num_css; i++) {
        pai_bound[4 * i + 0] = width; // min x
        pai_bound[4 * i + 1] = height; // min y
        pai_bound[4 * i + 2] = -1; // max x
        pai_bound[4 * i + 3] = -1; // max y

    }

    int i_mcurr;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            i_mcurr = indexmap[y * width + x] - 1;

			if (pai_bound[4 * i_mcurr + 0] > x) {
				pai_bound[4 * i_mcurr + 0] = x;
            }

			if (pai_bound[4 * i_mcurr + 1] > y) {
				pai_bound[4 * i_mcurr + 1] = y;
            }

			if (pai_bound[4 * i_mcurr + 2] < x) {
				pai_bound[4 * i_mcurr + 2] = x;
            }

			if (pai_bound[4 * i_mcurr + 3] < y) {
				pai_bound[4 * i_mcurr + 3] = y;
            }
        }
    }

    return pai_bound;
}

int *get_region_size(int *indexmap, int height, int width, int num_css)
{
    int *pai_sizes = new int[num_css];

    for (int i = 0; i < num_css; i++) {
		pai_sizes[i] = 0;
    }

    int i_mcurr;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            i_mcurr = indexmap[y * width + x] - 1;
			pai_sizes[i_mcurr]++;
        }
    }

	return pai_sizes;
}

ColorHistogram *get_region_ColorHistogram(image<rgb> *img, int *indexmap, int num_css)
{
    ColorHistogram *pao_chist = new ColorHistogram[num_css];

    for (int i = 0; i < num_css; i++) {
        for (int j = 0; j < COLOUR_BINS; j++) {
			pao_chist[i].mad_red[j] = 0.0;
			pao_chist[i].mad_green[j] = 0.0;
			pao_chist[i].mad_blue[j] = 0.0;
        }
    }

    int i_width = img->width();
	int i_height = img->height();
    float fl_step = 255.0 / COLOUR_BINS + 0.005; // divide the color value into COLOUR_BINS intervals

    int i_mcurr;
	int n_num;

	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {
			i_mcurr = indexmap[y * i_width + x] - 1;
			n_num = int(imRef(img, x, y).r / fl_step);

			pao_chist[i_mcurr].mad_red[n_num]++;
			n_num = int(imRef(img, x, y).g / fl_step);

			pao_chist[i_mcurr].mad_green[n_num]++;
			n_num = int(imRef(img, x, y).b / fl_step);

			pao_chist[i_mcurr].mad_blue[n_num]++;
        }
    }

    return pao_chist;
}

TextureHistogram *get_region_TextureHistogram(image<rgb> *im, int *indexmap, int num_css)
{
    TextureHistogram *pao_thist = new TextureHistogram[num_css];

    for (int i = 0; i < num_css; i++) {
        for (int j = 0; j < TEXTURE_DIM; j++) {
			pao_thist[i].mad_red[j] = 0.0;
			pao_thist[i].mad_green[j] = 0.0;
			pao_thist[i].mad_blue[j] = 0.0;
        }
    }


    int i_width = im->width();
	int i_height = im->height();

	image<float> *pofl_red = new image<float>(i_width, i_height);
	image<float> *pofl_green = new image<float>(i_width, i_height);
	image<float> *pofl_blue = new image<float>(i_width, i_height);

    // gaussian derivative
	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {
			imRef(pofl_red, x, y)   = imRef(im, x, y).r;
			imRef(pofl_green, x, y) = imRef(im, x, y).g;
			imRef(pofl_blue, x, y)  = imRef(im, x, y).b;
        }
    }

	image<float> *pofl_red_gauss = new image<float>(i_width, i_height);
	image<float> *pofl_green_gauss = new image<float>(i_width, i_height);
	image<float> *pofl_blue_gauss = new image<float>(i_width, i_height);

    int i_theta = 360 / DIRECTION_NUM; // the unit angel

    for (int i = 0; i < DIRECTION_NUM; i++) {
		anigauss(pofl_red->data, pofl_red_gauss->data, i_width, i_height, 1.0, 1.0, 1.0 * i * i_theta, 1, 1);
		anigauss(pofl_green->data, pofl_green_gauss->data, i_width, i_height, 1.0, 1.0, 1.0 * i * i_theta, 1, 1);
		anigauss(pofl_blue->data, pofl_blue_gauss->data, i_width, i_height, 1.0, 1.0, 1.0 * i * i_theta, 1, 1);

		float fl_r_max = NEG_INFIN;
		float fl_r_min = INFIN;
		float fl_g_max = NEG_INFIN;
		float fl_g_min = INFIN;
		float fl_b_max = NEG_INFIN;
		float fl_b_min = INFIN;

		for (int y = 0; y < i_height; y++) {
			for (int x = 0; x < i_width; x++) {

				if (imRef(pofl_red_gauss, x, y) > fl_r_max) { // r channel
					fl_r_max = imRef(pofl_red_gauss, x, y);
                }

				if (imRef(pofl_red_gauss, x, y) < fl_r_min) {
					fl_r_min = imRef(pofl_red_gauss, x, y);
                }

				if (imRef(pofl_green_gauss, x, y) > fl_g_max) { // g channel
					fl_g_max = imRef(pofl_green_gauss, x, y);
                }

				if (imRef(pofl_green_gauss, x, y) < fl_g_min) {
					fl_g_min = imRef(pofl_green_gauss, x, y);
                }

				if (imRef(pofl_blue_gauss, x, y) > fl_b_max) { // b channel
					fl_b_max = imRef(pofl_blue_gauss, x, y);
                }

				if (imRef(pofl_blue_gauss, x, y) < fl_b_min) {
					fl_b_min = imRef(pofl_blue_gauss, x, y);
                }
            }
        }

        float fl_r_step = (fl_r_max - fl_r_min) / (TEXTURE_BINS - 0.05);
		float fl_g_step = (fl_g_max - fl_g_min) / (TEXTURE_BINS - 0.05);
		float fl_b_step = (fl_b_max - fl_b_min) / (TEXTURE_BINS - 0.05);

		int i_mcurr;
        int n_num;

        for (int y = 0; y < i_height; y++) {
			for (int x = 0; x < i_width; x++) {
				i_mcurr = indexmap[y * i_width + x] - 1;
				n_num = int((imRef(pofl_red_gauss, x, y) - fl_r_min) / fl_r_step);
				pao_thist[i_mcurr].mad_red[DIRECTION_NUM * i + n_num]++;

				n_num = int((imRef(pofl_green_gauss, x, y) - fl_g_min) / fl_g_step);
				pao_thist[i_mcurr].mad_green[DIRECTION_NUM * i + n_num]++;

				n_num = int((imRef(pofl_blue_gauss, x, y) - fl_b_min) / fl_b_step);
				pao_thist[i_mcurr].mad_blue[DIRECTION_NUM * i + n_num]++;
            }
        }
    }

	delete pofl_red_gauss;
	delete pofl_green_gauss;
	delete pofl_blue_gauss;

	delete pofl_red;
	delete pofl_green;
	delete pofl_blue;

    return pao_thist;
}

