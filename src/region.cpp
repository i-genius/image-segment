/*
* @File: region.cpp
* @Author: tangxuan
*
* @Created on January 1, 2017, 11:33 PM
*/

#include "region.h"
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

Region::Region()
{
    mi_size = 0;

	for (int i = 0; i < 4; i++) {
		mai_bound[i] = 0;
	}
	/*mai_bound[0] = 0;
	mai_bound[1] = 0;
	mai_bound[2] = 0;
	mai_bound[3] = 0;*/

	mpo_self_subregion_a = NULL;
	mpo_self_subregion_b = NULL;
	mi_image_size = 1;
}

Region::Region(int image_size)
{
	mi_size = 0;

	for (int i = 0; i < 4; i++) {
		mai_bound[i] = 0;
	}
    /*mai_bound[0] = 0;
    mai_bound[1] = 0;
    mai_bound[2] = 0;
    mai_bound[3] = 0;*/

	mpo_self_subregion_a = NULL;
	mpo_self_subregion_b = NULL;
	this->mi_image_size = image_size;
}

Region Region::operator=(Region *region)
{
	this->mi_size = region->mi_size;
	for (int i = 0; i < 4; i++) {
		this->mai_bound[i] = region->mai_bound[i];
	}

	this->mo_chist = region->mo_chist;
    this->mo_thist = region->mo_thist;
	this->mpo_self_subregion_a = region->mpo_self_subregion_a;
	this->mpo_self_subregion_b = region->mpo_self_subregion_b;
	this->mi_image_size = region->mi_image_size;

    return *this;
}

Region Region::merge_egion(Region *region_1, Region *region_2)
{
	Region* merge_region = new Region(region_1->mi_image_size);

    // size
	merge_region->mi_size = region_1->mi_size + region_2->mi_size;

    // bounding box
    merge_region->mai_bound[0] = std::min(region_1->mai_bound[0], region_2->mai_bound[0]);
    merge_region->mai_bound[1] = std::min(region_1->mai_bound[1], region_2->mai_bound[1]);
    merge_region->mai_bound[2] = std::max(region_1->mai_bound[2], region_2->mai_bound[2]);
    merge_region->mai_bound[3] = std::max(region_1->mai_bound[3], region_2->mai_bound[3]);

    // ColorHistogram
    ColorHistogram* c1 = new ColorHistogram;
    ColorHistogram* c2 = new ColorHistogram;

	*c1 = region_1->mo_chist;
	*c2 = region_2->mo_chist;
	merge_region->mo_chist = (*c1 * region_1->mi_size + *c2 * region_2->mi_size) / (merge_region->mi_size);


    delete c1;
    delete c2;

    // TextureHistogram
    TextureHistogram* t1 = new TextureHistogram;
    TextureHistogram* t2 = new TextureHistogram;

	*t1 = region_1->mo_thist;
	*t2 = region_2->mo_thist;
	merge_region->mo_thist = (*t1 * region_1->mi_size + *t2 * region_2->mi_size) / (merge_region->mi_size);

    delete t1;
    delete t2;

    // subregion
	merge_region->mpo_self_subregion_a = region_1;
	merge_region->mpo_self_subregion_b = region_2;

    *this = *merge_region;
    delete merge_region;
    return *this;
}

double Region::similarity(Region* region)
{
	double sim_colour = this->mo_chist.similarity(&(region->mo_chist)) / 3.0;
	double sim_texture = this->mo_thist.similarity(&(region->mo_thist)) / (DIRECTION_NUM * 3.0);
	double s_image_size = this->mi_image_size - this->mi_size - region->mi_size;
	s_image_size /= this->mi_image_size;
    int rect = (this->mai_bound[2] - this->mai_bound[0])*(this->mai_bound[3] - this->mai_bound[1]);
	double sim_fill = this->mi_image_size - (rect - this->mi_size - region->mi_size);
	sim_fill /= this->mi_image_size;

    return sim_colour + sim_texture + s_image_size + sim_fill;
}
