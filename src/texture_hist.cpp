/* 
 * @File: texture_hist.cpp
 * @Author: tangxuan
 *
 * @Created on December 24, 2016, 9:34 PM
 */

//TODO: Rewrite the class inheritance method

#include <stdlib.h>
#include "texture_hist.h"
#include <algorithm>

TextureHistogram::TextureHistogram() 
{
    for (int i = 0; i < TEXTURE_DIM; i++) {
		mad_red[i]   = 0.0;
		mad_green[i] = 0.0;
		mad_blue[i]  = 0.0;
    }
}

TextureHistogram::~TextureHistogram()
{

}

TextureHistogram TextureHistogram::operator=(TextureHistogram *hist) 
{
    for (int i = 0; i < TEXTURE_DIM; i++) {
		this->mad_red[i]   = hist->mad_red[i];
		this->mad_green[i] = hist->mad_green[i];
		this->mad_blue[i]  = hist->mad_blue[i];
    }

    return *this; //TODO: rewrite here, return the pointer
}

TextureHistogram TextureHistogram::operator*(double scale) 
{
    for (int i = 0; i < TEXTURE_DIM; i++) {
		this->mad_red[i]   *= scale;
		this->mad_green[i] *= scale;
		this->mad_blue[i]  *= scale;
    }

    return *this; //TODO: rewrite here, return the pointer
}

TextureHistogram TextureHistogram::operator/(double num) 
{
    if (!num) return *this;
    return (*this * (1 / num));  //TODO: rewrite here, return the pointer
}

TextureHistogram operator+(TextureHistogram hist_1, TextureHistogram hist_2) 
{
    TextureHistogram *po_histogram = new TextureHistogram; // a new TextureHistogram;
    for (int i = 0; i < TEXTURE_DIM; i++) {
		po_histogram->mad_red[i]   = hist_1.mad_red[i]   + hist_2.mad_red[i];
		po_histogram->mad_green[i] = hist_1.mad_green[i] + hist_2.mad_green[i];
		po_histogram->mad_blue[i]  = hist_1.mad_blue[i]  + hist_2.mad_blue[i];
    }

	return *po_histogram; //TODO: rewrite here, return the pointer
}

double TextureHistogram::similarity(TextureHistogram *hist) {
    double d_sim = 0;
    for (int i = 0; i < TEXTURE_DIM; i++) {
		d_sim += std::min(this->mad_red[i],   hist->mad_red[i]);
		d_sim += std::min(this->mad_green[i], hist->mad_green[i]);
		d_sim += std::min(this->mad_blue[i],  hist->mad_blue[i]);
    }

	return d_sim;
}
