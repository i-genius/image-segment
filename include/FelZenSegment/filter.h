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

#ifndef __FILTER_H__
#define __FILTER_H__

#include <vector>
#include <algorithm>

#include "image.h"
#include "misc.h"

#define WIDTH 4.0

/* normalize mask so it integrates to one */
void normalize(std::vector<float> &mask);

/* convolve image with gaussian filter */
image<float> *smooth(image<float> *src, float sigma);

/* convolve image with gaussian filter */
image<float> *smooth(image<uchar> *src, float sigma);

/* compute laplacian */
image<float> *laplacian(image<float> *src);

#endif
