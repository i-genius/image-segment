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

/* simple filters */

#include <math.h>
#include "convolve.h"
#include "imconv.h"

#include "filter.h"

#define WIDTH 4.0

/* normalize mask so it integrates to one */
void normalize(std::vector<float> &mask)
{
	int n_len = mask.size();
	float fl_sum = 0;

	for (int i = 1; i < n_len; i++) {
		fl_sum += fabs(mask[i]);
	}

	fl_sum = 2 * fl_sum + fabs(mask[0]);

	for (int i = 0; i < n_len; i++) {
		mask[i] /= fl_sum;
	}
}

/* make filters */
#define MAKE_FILTER(name, fun)                                \
  std::vector<float> make_ ## name (float sigma) {			  \
  sigma = std::max(sigma, 0.01F);							  \
  int len = (int)ceil(sigma * WIDTH) + 1;                     \
  std::vector<float> mask(len);                               \
  for (int i = 0; i < len; i++) {                             \
    mask[i] = fun;                                            \
      }                                                       \
  return mask;                                                \
}

MAKE_FILTER(fgauss, exp(-0.5 * square(i / sigma)));


/* convolve image with gaussian filter */
image<float> *smooth(image<float> *src, float sigma)
{
	std::vector<float> mask = make_fgauss(sigma);
	normalize(mask);

	image<float> *pofl_tmp = new image<float>(src->height(), src->width(), false);
	image<float> *pofl_dst = new image<float>(src->width(), src->height(), false);
	convolve_even(src, pofl_tmp, mask);
	convolve_even(pofl_tmp, pofl_dst, mask);

	delete pofl_tmp;
	return pofl_dst;
}

/* convolve image with gaussian filter */
image<float> *smooth(image<uchar> *src, float sigma)
{
	image<float> *pofl_tmp = imageUCHARtoFLOAT(src);
	image<float> *pofl_dst = smooth(pofl_tmp, sigma);
	delete pofl_tmp;
	return pofl_dst;
}

/* compute laplacian */
image<float> *laplacian(image<float> *src)
{
	int i_width = src->width();
	int i_height = src->height();
	image<float> *pofl_dst = new image<float>(i_width, i_height);

	for (int y = 1; y < i_height - 1; y++) {
		for (int x = 1; x < i_width - 1; x++) {
			float fl_d2x = imRef(src, x - 1, y) + imRef(src, x + 1, y) -
				2 * imRef(src, x, y);
			float fl_d2y = imRef(src, x, y - 1) + imRef(src, x, y + 1) -
				2 * imRef(src, x, y);
			imRef(pofl_dst, x, y) = fl_d2x + fl_d2y;
		}
	}

	return pofl_dst;
}

