/*
* @File: convert.h
* @Author: tangxuan
*
* @Created on December 31, 2016, 6:27 PM
*/

//Note: __attribute__((deprecated))

#ifndef __TYPE_CONVERT_H__
#define __TYPE_CONVERT_H__

#include "image.h"
#include "misc.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

image<rgb> *mat2Image(cv::Mat& mat);

cv::Mat *image2Mat(image<rgb> *im);

#endif
