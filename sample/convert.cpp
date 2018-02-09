/*
* @File: convert.cpp
* @Author: tangxuan
*
* @Created on December 31, 2016, 6:27 PM
*/

//Note: __attribute__((deprecated))

#include "convert.h"

image<rgb> *mat2Image(cv::Mat& mat)
{
	int n_channel = mat.channels();

	if (n_channel < 3){
		fprintf(stderr, "Image type error. Please use  3 channels image(rgb)....");
		//exit(1);
		return NULL;
	}

	int i_width = mat.cols;
	int i_height = mat.rows;

	image<rgb> *im = new image<rgb>(i_width, i_height);  //not delete memory here

	for (int y = 0; y < i_height; y++){
		for (int x = 0; x < i_width; x++){
			uchar *p = mat.ptr<uchar>(y);
			imRef(im, x, y).b = p[x * n_channel + 0];
			imRef(im, x, y).g = p[x * n_channel + 1];
			imRef(im, x, y).r = p[x * n_channel + 2];
		}
	}

	return im;
}

cv::Mat *image2Mat(image<rgb>* im)
{
	int i_width = im->width();
	int i_height = im->height();

	cv::Mat* mat = new cv::Mat(cv::Size(i_width, i_height), CV_8UC3);  //not delete memory here
	int n_channel = mat->channels();    //here channel is always 3

	for (int y = 0; y < i_height; y++){
		for (int x = 0; x < i_width; x++){
			uchar* p = mat->ptr<uchar>(y);
			p[x * n_channel + 0] = imRef(im, x, y).b;
			p[x * n_channel + 1] = imRef(im, x, y).g;
			p[x * n_channel + 2] = imRef(im, x, y).r;
		}
	}

	return mat;
}
