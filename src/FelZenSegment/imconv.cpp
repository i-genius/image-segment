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

/* image conversion */

#include <limits.h>
#include "imutil.h"
#include "imconv.h"

image<uchar> *imageRGBtoGRAY(image<rgb> *input)
{
	int i_width = input->width();
	int i_height = input->height();
	image<uchar> *pouc_output = new image<uchar>(i_width, i_height, false);

	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {
			imRef(pouc_output, x, y) = (uchar)
				(imRef(input, x, y).r * RED_WEIGHT +
				imRef(input, x, y).g * GREEN_WEIGHT +
				imRef(input, x, y).b * BLUE_WEIGHT);
		}
	}

	return pouc_output;
}

image<rgb> *imageGRAYtoRGB(image<uchar> *input)
{
	int i_width = input->width();
	int i_height = input->height();
	image<rgb> *po_output = new image<rgb>(i_width, i_height, false);

	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {
			imRef(po_output, x, y).r = imRef(input, x, y);
			imRef(po_output, x, y).g = imRef(input, x, y);
			imRef(po_output, x, y).b = imRef(input, x, y);
		}
	}
	return po_output;
}

image<float> *imageUCHARtoFLOAT(image<uchar> *input)
{
	int i_width = input->width();
	int i_height = input->height();
	image<float> *pofl_output = new image<float>(i_width, i_height, false);

	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {
			imRef(pofl_output, x, y) = imRef(input, x, y);
		}
	}
	return pofl_output;
}

image<float> *imageINTtoFLOAT(image<int> *input)
{
	int i_width = input->width();
	int i_height = input->height();
	image<float> *pofl_output = new image<float>(i_width, i_height, false);

	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {
			imRef(pofl_output, x, y) = imRef(input, x, y);
		}
	}
	return pofl_output;
}

image<uchar> *imageFLOATtoUCHAR(image<float> *input, float min, float max)
{
	int i_width = input->width();
	int i_height = input->height();
	image<uchar> *pouc_output = new image<uchar>(i_width, i_height, false);

	if (max == min)
		return pouc_output;

	float fl_scale = UCHAR_MAX / (max - min);
	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {
			uchar uc_val = (uchar)((imRef(input, x, y) - min) * fl_scale);
			imRef(pouc_output, x, y) = bound(uc_val, (uchar)0, (uchar)UCHAR_MAX);
		}
	}
	return pouc_output;
}

image<uchar> *imageFLOATtoUCHAR(image<float> *input)
{
	float fl_min, fl_max;
	min_max(input, &fl_min, &fl_max);
	return imageFLOATtoUCHAR(input, fl_min, fl_max);
}

image<long> *imageUCHARtoLONG(image<uchar> *input)
{
	int i_width = input->width();
	int i_height = input->height();
	image<long> *pol_output = new image<long>(i_width, i_height, false);

	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {
			imRef(pol_output, x, y) = imRef(input, x, y);
		}
	}
	return pol_output;
}

image<uchar> *imageLONGtoUCHAR(image<long> *input, long min, long max)
{
	int i_width = input->width();
	int i_height = input->height();
	image<uchar> *pouc_output = new image<uchar>(i_width, i_height, false);

	if (max == min)
		return pouc_output;

	float fl_scale = UCHAR_MAX / (float)(max - min);
	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {
			uchar uc_val = (uchar)((imRef(input, x, y) - min) * fl_scale);
			imRef(pouc_output, x, y) = bound(uc_val, (uchar)0, (uchar)UCHAR_MAX);
		}
	}
	return pouc_output;
}

image<uchar> *imageLONGtoUCHAR(image<long> *input)
{
	long l_min, l_max;
	min_max(input, &l_min, &l_max);
	return imageLONGtoUCHAR(input, l_min, l_max);
}

image<uchar> *imageSHORTtoUCHAR(image<short> *input, short min, short max)
{
	int i_width = input->width();
	int i_height = input->height();
	image<uchar> *pouc_output = new image<uchar>(i_width, i_height, false);

	if (max == min)
		return pouc_output;

	float fl_scale = UCHAR_MAX / (float)(max - min);
	for (int y = 0; y < i_height; y++) {
		for (int x = 0; x < i_width; x++) {
			uchar uc_val = (uchar)((imRef(input, x, y) - min) * fl_scale);
			imRef(pouc_output, x, y) = bound(uc_val, (uchar)0, (uchar)UCHAR_MAX);
		}
	}
	return pouc_output;
}

image<uchar> *imageSHORTtoUCHAR(image<short> *input)
{
	short s_min, s_max;
	min_max(input, &s_min, &s_max);
	return imageSHORTtoUCHAR(input, s_min, s_max);
}
