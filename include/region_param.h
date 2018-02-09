/*
* @File: region_param.h
* @Author: tangxuan
*
* @Created on January 1, 2017, 5:11 PM
*/

#ifndef __REGION_PARAM_H__
#define __REGION_PARAM_H__

#include "color_hist.h"
#include "texture_hist.h"
#include "image.h"
#include "misc.h"

/*
 * Segment an image
 *
 * Returns a color image representing the segmentation. 
 *
 * im: image to segment.
 * sigma: to smooth the image.
 * c: constant for treshold function.
 * min_size: minimum component size (enforced by post-processing stage).
 * num_ccs: number of connected components in the segmentation.
 */
int *segment_image_index(image<rgb> *im, float sigma, float c, int min_size, int *num_ccs);


/*
 * Tack neighbors of the regions
 *
 * Returns an array (num_css * num_css) to label the realationship of neighborhood.
 *
 * imdexmap: indexmap of image.
 * height: height of image.
 * width: width of image.
 * num_ccs: number of connected components in the segmentation.
 */
// TODO: 
int *track_region_neighbors(int *indexmap, int height, int width, int num_css);      /* check there  2017.01.07 20:30 */


/*
 * Get the minimum and maximum of region -- bounding box
 *
 * Returns an array (num_css * 4) for four parameters of bounding box
 *
 * imdexmap: indexmap of image.
 * height: height of image.
 * width: width of image.
 * num_ccs: number of connected components in the segmentation.
 */
int *get_region_bound(int *indexmap, int height, int width, int num_css);


/*
 * Get the number of pixels in each region
 *
 * Returns an array (num_css) for size of region
 *
 * imdexmap: indexmap of image.
 * height: height of image.
 * width: width of image.
 * num_ccs: number of connected components in the segmentation.
 */
int *get_region_size(int *indexmap, int height, int width, int num_css);


/*
 * Get the color histogram of each region
 *
 * Returns an array (num_css) for color histogram of region
 *
 * im: original image.
 * imdexmap: indexmap of image.
 * num_ccs: number of connected components in the segmentation.
 */
ColorHistogram *get_region_ColorHistogram(image<rgb> *img, int *indexmap, int num_css);


/*
 * Get the texture histogram of each region
 *
 * Returns an array (num_css) for size of region
 *
 * im: original image.
 * imdexmap: indexmap of image.
 * num_ccs: number of connected components in the segmentation.
 */
TextureHistogram *get_region_TextureHistogram(image<rgb> *im, int *indexmap, int num_css);

#endif
