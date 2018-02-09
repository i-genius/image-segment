/*
* @File: color_hist.cpp
* @Author: tangxuan
*
* @Created on December 24, 2016, 7:34 PM
*/

//TODO: Rewrite the class inheritance method

#include <algorithm>
#include "color_hist.h"

ColorHistogram::ColorHistogram() 
{
    for (int i = 0; i < COLOUR_BINS; i++) {
		mad_red[i]   = 0.0;
		mad_green[i] = 0.0;
		mad_blue[i]  = 0.0;
    }
}

ColorHistogram::~ColorHistogram()
{

}

ColorHistogram ColorHistogram::operator=(ColorHistogram *hist)
{
    for (int i = 0; i < COLOUR_BINS; i++) {
		this->mad_red[i]   = hist->mad_red[i];
		this->mad_green[i] = hist->mad_green[i];
		this->mad_blue[i]  = hist->mad_blue[i];
    }

    return *this; //TODO: rewrite here, return the pointer
}

ColorHistogram ColorHistogram::operator*(double scale) 
{
    for (int i = 0; i < COLOUR_BINS; i++) {
		this->mad_red[i]   *= scale;
		this->mad_green[i] *= scale;
		this->mad_blue[i]  *= scale;
    }

    return *this; //TODO: rewrite here, return the pointer
}

ColorHistogram ColorHistogram::operator/(double num) 
{
    if (!num) return *this;
    return (*this * (1 / num)); //TODO: rewrite here, return the pointer
}

ColorHistogram operator+(ColorHistogram hist_1, ColorHistogram hist_2) 
{
    ColorHistogram *po_histogram = new ColorHistogram; // a new ColorHistogram;
    for (int i = 0; i < COLOUR_BINS; i++) {
		po_histogram->mad_red[i]   = hist_1.mad_red[i]   + hist_2.mad_red[i];
		po_histogram->mad_green[i] = hist_1.mad_green[i] + hist_2.mad_green[i];
		po_histogram->mad_blue[i]  = hist_1.mad_blue[i]  + hist_2.mad_blue[i];
    }

	return *po_histogram;  //TODO: rewrite here, return the pointer
}

double ColorHistogram::similarity(ColorHistogram *hist) 
{
    double d_sim = 0;
    for (int i = 0; i < COLOUR_BINS; i++) {
		d_sim += std::min(this->mad_red[i],   hist->mad_red[i]);
		d_sim += std::min(this->mad_green[i], hist->mad_green[i]);
		d_sim += std::min(this->mad_blue[i],  hist->mad_blue[i]);
    }

    return d_sim;
}

