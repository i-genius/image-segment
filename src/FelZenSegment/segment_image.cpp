/*
Copyright (C) 2006 Pedro Felzenszwalb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

#include <stdlib.h>
#include "filter.h"
#include "segment_graph.h"
#include "segment_image.h"

// random color
rgb random_rgb()
{
	rgb o_color;

	o_color.r = (uchar)rand();
	o_color.g = (uchar)rand();
	o_color.b = (uchar)rand();

	return o_color;
}

// dissimilarity measure between pixels
float diff(image<float> *r, image<float> *g, image<float> *b, int x1, int y1, int x2, int y2)
{
	return sqrt(square(imRef(r, x1, y1) - imRef(r, x2, y2)) +
		        square(imRef(g, x1, y1) - imRef(g, x2, y2)) +
		        square(imRef(b, x1, y1) - imRef(b, x2, y2)));
}

/*
* Segment an image
*
* Returns a color image representing the segmentation.
*
* im: image to segment.
* sigma: to smooth the image.
* fl_con: constant for treshold function.
* min_size: minimum component size (enforced by post-processing stage).
* num_ccs: number of connected components in the segmentation.
*/
image<rgb> *segment_image(image<rgb> *im, float sigma, float fl_con, int min_size, int *num_ccs)
{
	int i_width = im->width();
	int i_height = im->height();

	image<float> *pofl_r = new image<float>(i_width, i_height);
	image<float> *pofl_g = new image<float>(i_width, i_height);
	image<float> *pofl_b = new image<float>(i_width, i_height);

	// smooth each color channel  
	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {
			imRef(pofl_r, x, y) = imRef(im, x, y).r;
			imRef(pofl_g, x, y) = imRef(im, x, y).g;
			imRef(pofl_b, x, y) = imRef(im, x, y).b;
		}
	}

	image<float> *pofl_smooth_r = smooth(pofl_r, sigma);
	image<float> *pofl_smooth_g = smooth(pofl_g, sigma);
	image<float> *pofl_smooth_b = smooth(pofl_b, sigma);

	delete pofl_r;
	delete pofl_g;
	delete pofl_b;

	// build graph
	edge *pao_edges = new edge[i_width * i_height * 4];
	int n_num = 0;

	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {

			if (x < i_width - 1) {
				pao_edges[n_num].a = y * i_width + x;
				pao_edges[n_num].b = y * i_width + (x + 1);
				pao_edges[n_num].w = diff(pofl_smooth_r, pofl_smooth_g, pofl_smooth_b, x, y, x + 1, y);
				n_num++;
			}

			if (y < i_height - 1) {
				pao_edges[n_num].a = y * i_width + x;
				pao_edges[n_num].b = (y + 1) * i_width + x;
				pao_edges[n_num].w = diff(pofl_smooth_r, pofl_smooth_g, pofl_smooth_b, x, y, x, y + 1);
				n_num++;
			}

			if ((x < i_width - 1) && (y < i_height - 1)) {
				pao_edges[n_num].a = y * i_width + x;
				pao_edges[n_num].b = (y + 1) * i_width + (x + 1);
				pao_edges[n_num].w = diff(pofl_smooth_r, pofl_smooth_g, pofl_smooth_b, x, y, x + 1, y + 1);
				n_num++;
			}

			if ((x < i_width - 1) && (y > 0)) {
				pao_edges[n_num].a = y * i_width + x;
				pao_edges[n_num].b = (y - 1) * i_width + (x + 1);
				pao_edges[n_num].w = diff(pofl_smooth_r, pofl_smooth_g, pofl_smooth_b, x, y, x + 1, y - 1);
				n_num++;
			}
		}
	}
	delete pofl_smooth_r;
	delete pofl_smooth_g;
	delete pofl_smooth_b;

	// segment
	Universe *po_uni = segment_graph(i_width * i_height, n_num, pao_edges, fl_con);

	// post process small components
	for (int i = 0; i < n_num; i++) {
		int i_a = po_uni->find(pao_edges[i].a);
		int i_b = po_uni->find(pao_edges[i].b);
		if ((i_a != i_b) && ((po_uni->size(i_a) < min_size) || (po_uni->size(i_b) < min_size))) {
			po_uni->join(i_a, i_b);
		}
	}

	delete[] pao_edges;
	*num_ccs = po_uni->num_sets();

	image<rgb> *po_im_output = new image<rgb>(i_width, i_height);

	// pick random colors for each component
	rgb *pao_colors = new rgb[i_width * i_height];

	for (int i = 0; i < i_width* i_height; i++) {
		pao_colors[i] = random_rgb();
	}

	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {
			int i_comp = po_uni->find(y * i_width + x);
			imRef(po_im_output, x, y) = pao_colors[i_comp];
		}
	}

	delete[] pao_colors;
	delete po_uni;

	return po_im_output;
}
