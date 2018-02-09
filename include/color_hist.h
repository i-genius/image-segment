/*
* @File: color_hist.h
* @Author: tangxuan
*
* @Created on May 14, 2017, 7:34 PM
*/

//TODO: Rewrite the class inheritance method

#ifndef __COLORHISTOGRAM_H__
#define __COLORHISTOGRAM_H__

#define COLOUR_BINS 25

class ColorHistogram{
public:
	ColorHistogram();
	~ColorHistogram();
	ColorHistogram operator=(ColorHistogram *hist);
	ColorHistogram operator*(double scale);
	ColorHistogram operator/(double num);
	friend ColorHistogram operator+(ColorHistogram hist_1, ColorHistogram hist_2);
	double similarity(ColorHistogram *hist);
	
public:
	double mad_red[COLOUR_BINS];
	double mad_green[COLOUR_BINS];
	double mad_blue[COLOUR_BINS];
};

#endif
