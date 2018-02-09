/*
* @File: type_convert.h
* @Author: tangxuan
*
* @Created on December 24, 2016, 9:34 PM
*/

#ifndef __TYPE_CONVERT_H__
#define __TYPE_CONVERT_H__

#include "image.h"
#include "misc.h"
#include "pnmfile.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

//using namespace cv;

image<rgb>* mat2Image(cv::Mat& I)
{
	int channel = I.channels();
	if (channel < 3){
		fprintf(stderr, "Image type error. Please use  3 channels image(rgb)....");
		exit(1);
	}

	int width = I.cols;
	int height = I.rows;

	image<rgb>* img = new image<rgb>(width, height);

	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			uchar* p = I.ptr<uchar>(y);
			imRef(img, x, y).b = p[x*channel + 0];
			imRef(img, x, y).g = p[x*channel + 1];
			imRef(img, x, y).r = p[x*channel + 2];
		}
	}

	return img;
}

cv::Mat* image2Mat(image<rgb>* img)
{
	int width = img->width();
	int height = img->height();

	cv::Mat* I = new cv::Mat(cv::Size(width, height), CV_8UC3);
	int channel = I->channels();
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			uchar* p = I->ptr<uchar>(y);
			p[x*channel + 0] = imRef(img, x, y).b;
			p[x*channel + 1] = imRef(img, x, y).g;
			p[x*channel + 2] = imRef(img, x, y).r;
		}
	}

	return I;
}

#endif
