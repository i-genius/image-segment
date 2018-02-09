/*
* @File: texture_hist.h
* @Author: tangxuan
*
* @Created on December 24, 2016, 9:34 PM
*/

//TODO: Rewrite the class inheritance method

#ifndef __TEXTUREHISTOGRAM_H__
#define __TEXTUREHISTOGRAM_H__

#define TEXTURE_BINS 10
#define DIRECTION_NUM 8
#define TEXTURE_DIM TEXTURE_BINS*DIRECTION_NUM

class TextureHistogram{
public:
	TextureHistogram();	
	~TextureHistogram();
	TextureHistogram operator=(TextureHistogram *hist);	
	TextureHistogram operator*(double scale);
	TextureHistogram operator/(double num);
	friend TextureHistogram operator+(TextureHistogram hist_1, TextureHistogram hist_2);
	double similarity(TextureHistogram *hist);	

public:
	double mad_red[TEXTURE_DIM];
	double mad_green[TEXTURE_DIM];
	double mad_blue[TEXTURE_DIM];

};

#endif
