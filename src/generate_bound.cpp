#include "generate_bound.h"
#include "color_hist.h"
#include "texture_hist.h"
#include "region.h"
#include "region_param.h"

#include "segment_image.h"
#include "pnmfile.h"

#include <stdlib.h>
#include <stdio.h>

int *generate_bounding_boxes(image<rgb> *im, float sigma, float fl_con, int min_size, int *nums)
{
	int i_width = im->width();
	int i_height = im->height();
	int i_num_css = 0;  

#if DEBUG_FLAG
	printf("Preprocessing...\n");
#endif

	/* Should consider the case of failure? */
	int *pai_indexmap = segment_image_index(im, sigma, fl_con, min_size, &i_num_css);   // height * width
	int *pai_track_area = track_region_neighbors(pai_indexmap, i_height, i_width, i_num_css);      // num_css * num_css
	int *pai_get_bound = get_region_bound(pai_indexmap, i_height, i_width, i_num_css);               // num_css * 4
	int *pai_sizes = get_region_size(pai_indexmap, i_height, i_width, i_num_css);          // num_css

	ColorHistogram *pao_chist = get_region_ColorHistogram(im, pai_indexmap, i_num_css);       // num_css
	TextureHistogram *pao_thist = get_region_TextureHistogram(im, pai_indexmap, i_num_css);      // num_css

	image<rgb> *po_im_out = new image<rgb>(i_width, i_height);

#if DEBUG_FLAG
	printf("Save the segments ...\n");
	char ac_img_name[200];   //when save result, 's' storage image name
#endif

	rgb *pao_colors = new rgb[i_num_css];

	for (int i = 0; i < i_num_css; i++) {
		pao_colors[i] = random_rgb();
	}

	// Save the color image regions
	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {
			imRef(po_im_out, x, y) = pao_colors[pai_indexmap[y * i_width + x]];
		}
	}

#if DEBUG_FLAG
	sprintf(ac_img_name, "../../data/result/regions.PPM");
	savePPM(po_im_out, ac_img_name);
#endif

	// Save the components
	for (int i = 0; i < i_num_css; i++) {
		for (int y = 0; y < i_height; y++) {
			for (int x = 0; x < i_width; x++) {

				if (pai_indexmap[y * i_width + x] == i + 1){
					imRef(po_im_out, x, y) = imRef(im, x, y);
				}
				else{
					imRef(po_im_out, x, y).r = (uchar)0;
					imRef(po_im_out, x, y).g = (uchar)0;
					imRef(po_im_out, x, y).b = (uchar)0;
				}
			}
		}

#if DEBUG_FLAG
		sprintf(ac_img_name, "../../data/regions/region_%d.PPM", i);
		savePPM(po_im_out, ac_img_name);
#endif
	}

	int i_region_nums = 2 * i_num_css - 1;
	*nums = i_region_nums;
	Region *pao_regions = new Region[i_region_nums];
	int *pai_area = new int[i_region_nums * i_region_nums];
	double *pad_region_sim = new double[i_region_nums * i_region_nums];

	// Initialize the regions
	int img_size = i_height * i_width;

	for (int i = 0; i < i_region_nums; i++) {
		pao_regions[i].mi_image_size = img_size;

		if (i < i_num_css){
			pao_regions[i].mi_size = pai_sizes[i];
			pao_regions[i].mai_bound[0] = pai_get_bound[4 * i + 0];
			pao_regions[i].mai_bound[1] = pai_get_bound[4 * i + 1];
			pao_regions[i].mai_bound[2] = pai_get_bound[4 * i + 2];
			pao_regions[i].mai_bound[3] = pai_get_bound[4 * i + 3];
			pao_regions[i].mo_chist = pao_chist[i];
			//pao_regions[i].mo_chist / pai_sizes[i];     //what effect? 
			pao_regions[i].mo_thist = pao_thist[i];
			//pao_regions[i].mo_thist / pai_sizes[i];    //what effect? 
		}
		else{
			pao_regions[i].mi_size = 0;
			pao_regions[i].mai_bound[0] = 0;
			pao_regions[i].mai_bound[1] = 0;
			pao_regions[i].mai_bound[2] = 0;
			pao_regions[i].mai_bound[3] = 0;

			for (int j = 0; j < COLOUR_BINS; j++){
				pao_regions[i].mo_chist.mad_red[j] = 0.0;
				pao_regions[i].mo_chist.mad_green[j] = 0.0;
				pao_regions[i].mo_chist.mad_blue[j] = 0.0;
			}

			for (int j = 0; j < TEXTURE_DIM; j++){
				pao_regions[i].mo_thist.mad_red[j] = 0.0;
				pao_regions[i].mo_thist.mad_green[j] = 0.0;
				pao_regions[i].mo_thist.mad_blue[j] = 0.0;
			}
		}
	}

	// copy the values in track_area to area (expand num_css * num_css to region_nums * region_nums)
	for (int i = 0; i < i_region_nums; i++) {
		for (int j = i; j < i_region_nums; j++) {

			if (i < i_num_css && j < i_num_css) {
				pai_area[i * i_region_nums + j] = pai_track_area[i * i_num_css + j];
				pai_area[j * i_region_nums + i] = pai_track_area[j * i_num_css + i];
			}
			else{
				pai_area[i * i_region_nums + j] = 0;
				pai_area[j * i_region_nums + i] = 0;
			}
		}
	}

	// calculate the similarities between neighbor regions
	for (int i = 0; i < i_region_nums; i++) {
		for (int j = i; j < i_region_nums; j++) {

			if (pai_area[i * i_region_nums + j] == 1) {
				pad_region_sim[i * i_region_nums + j] = pao_regions[i].similarity(&pao_regions[j]);
				pad_region_sim[j * i_region_nums + i] = pad_region_sim[i * i_region_nums + j];
			}
			else{
				pad_region_sim[i * i_region_nums + j] = 0.0;
				pad_region_sim[j * i_region_nums + i] = 0.0;
			}
		}
	}

	delete[] pai_track_area;
	delete[] pai_get_bound;
	delete[] pai_sizes;

	printf("Start to merge the segments ...\n");

	for (int k = i_num_css; k < i_region_nums; k++) {
		// find the pairs (R[i],R[j])with max similarity
		double d_max_sim = 0.0;
		int n_sim_index_i = 0;
		int n_sim_index_j = 0;

		for (int i = 0; i < i_region_nums; i++){
			for (int j = i + 1; j < i_region_nums; j++){
				int idx = i * i_region_nums + j;

				if (pad_region_sim[idx] > d_max_sim){
					d_max_sim = pad_region_sim[idx];
					n_sim_index_i = i;
					n_sim_index_j = j;
				}
			}
		}

#if DEBUG_FLAG
		printf("[%4d,(%4d,%4d;%4d,%4d):%6d] [%4d,(%4d,%4d;%4d,%4d),%6d]--->(%4d,%.2lf).\n",
			n_sim_index_i, 
			pao_regions[n_sim_index_i].mai_bound[0], 
			pao_regions[n_sim_index_i].mai_bound[1], 
			pao_regions[n_sim_index_i].mai_bound[2], 
			pao_regions[n_sim_index_i].mai_bound[3], 
			pao_regions[n_sim_index_i].mi_size,

			n_sim_index_j, 
			pao_regions[n_sim_index_j].mai_bound[0],
			pao_regions[n_sim_index_j].mai_bound[1], 
			pao_regions[n_sim_index_j].mai_bound[2], 
			pao_regions[n_sim_index_j].mai_bound[3], 
			pao_regions[n_sim_index_j].mi_size,

			k, pad_region_sim[n_sim_index_i * i_region_nums + n_sim_index_j]);
#endif

		// merge these two regions
		pao_regions[k].merge_egion(&pao_regions[n_sim_index_i], &pao_regions[n_sim_index_j]);

		for (int y = 0; y < i_height; y++) {
			for (int x = 0; x < i_width; x++) {

				if ((pai_indexmap[y * i_width + x] == n_sim_index_i + 1) || (pai_indexmap[y * i_width + x] == n_sim_index_j + 1)){
					imRef(po_im_out, x, y) = imRef(im, x, y);
					pai_indexmap[y * i_width + x] = k + 1;
				}
				else{
					imRef(po_im_out, x, y).r = (uchar)0;
					imRef(po_im_out, x, y).g = (uchar)0;
					imRef(po_im_out, x, y).b = (uchar)0;
				}
			}
		}

#if DEBUG_FLAG
		sprintf(ac_img_name, "../../data/regions_2/region_%d.PPM", k);
		savePPM(po_im_out, ac_img_name);
#endif

		// update the neighbors of new region and the similarity matrix
		for (int i = 0; i < i_region_nums; i++) {

			if (!(i == n_sim_index_i || i == n_sim_index_j) && 
					((pai_area[n_sim_index_i * i_region_nums + i] == 1) || 
					(pai_area[n_sim_index_j * i_region_nums + i] == 1))) {

				pai_area[k * i_region_nums + i] = 1;
				pai_area[i * i_region_nums + k] = 1;
				pad_region_sim[k * i_region_nums + i] = pao_regions[k].similarity(&pao_regions[i]);
				pad_region_sim[i * i_region_nums + k] = pad_region_sim[k * i_region_nums + i];
			}
		}


		for (int i = 0; i < i_region_nums; i++){
			pai_area[n_sim_index_i * i_region_nums + i] = 0;
			pai_area[i * i_region_nums + n_sim_index_i] = 0;
			pad_region_sim[n_sim_index_i * i_region_nums + i] = 0;
			pad_region_sim[i * i_region_nums + n_sim_index_i] = 0;

			pai_area[n_sim_index_j * i_region_nums + i] = 0;
			pai_area[i * i_region_nums + n_sim_index_j] = 0;
			pad_region_sim[n_sim_index_j * i_region_nums + i] = 0;
			pad_region_sim[i * i_region_nums + n_sim_index_j] = 0;
		}

	}

	int *pai_bound = new int[i_region_nums * 4];

	for (int i = 0; i < i_region_nums; i++) {
		pai_bound[4 * i + 0] = pao_regions[i].mai_bound[0];   //min x
		pai_bound[4 * i + 1] = pao_regions[i].mai_bound[1];   //min y
		pai_bound[4 * i + 2] = pao_regions[i].mai_bound[2];   //max x
		pai_bound[4 * i + 3] = pao_regions[i].mai_bound[3];   //max y
	}

	delete po_im_out;
	delete pai_indexmap;
	delete[] pao_regions;
	delete[] pai_area;
	delete[] pad_region_sim;

	return pai_bound;
}
