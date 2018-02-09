/*
* @File: generate_bound.h
* @Author: tangxuan
*
* @Created on January 1, 2017, 8:09 PM
*/

#ifndef __GENERATE_BOUND_H__
#define __GENERATE_BOUND_H__

#include "image.h"
#include "misc.h"

#ifdef _OUTPUT_DEBUG
#define DEBUG_FLAG 1
#else
#define DEBUG_FLAG 0
#endif
/*
 * Generate bounding boxes for input image
 *
 * Returns a series of bound boxing parameters, two point (xmin,ymin)(xmax,ymax).
 *
 * im: input image.
 * sigma: to smooth the image.
 * fl_con: constant for treshold function.
 * min_size: minimum component size (enforced by post-processing stage).
 */
int *generate_bounding_boxes(image<rgb> *im, float sigma, float fl_con, int min_size, int *nums);

#endif
