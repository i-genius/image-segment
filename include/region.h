/*
* @File: region.h
* @Author: tangxuan
*
* @Created on January 1, 2017, 11:33 PM
*/

#ifndef __REGION_H__
#define __REGION_H__

#include "color_hist.h"
#include "texture_hist.h"

class Region{
public:
	Region();
	Region(int image_size);
	Region operator=(Region* region);
	Region merge_egion(Region *region_1, Region *region_2);
	double similarity(Region* region);

public:
	int mi_size;
	int mai_bound[4];
	ColorHistogram mo_chist;
	TextureHistogram mo_thist;
	Region *mpo_self_subregion_a;
	Region *mpo_self_subregion_b;
	int mi_image_size;
};

#endif
