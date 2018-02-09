/*
Copyright (C) 2006 Pedro Felzenszwalb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

/* basic image I/O */

#ifndef __PNM_FILE_H__
#define __PNM_FILE_H__

#include <fstream>
#include "image.h"
#include "misc.h"

#define BUF_SIZE 256  //2^8

class pnm_error { }; //

void read_packed(unsigned char *data, int size, std::ifstream &f);
void write_packed(unsigned char *data, int size, std::ofstream &f);

/* read PNM field, skipping comments */
void pnm_read(std::ifstream &file, char *buf);

image<uchar> *loadPBM(const char *name);

void savePBM(image<uchar> *im, const char *name);

image<uchar> *loadPGM(const char *name);

void savePGM(image<uchar> *im, const char *name);

image<rgb> *loadPPM(const char *name);

void savePPM(image<rgb> *im, const char *name);

//template class
template <class T>
void load_image(image<T> **im, const char *name) {
	char buf[BUF_SIZE];

	/* read header */
	std::ifstream file(name, std::ios::in | std::ios::binary);
	pnm_read(file, buf);
	if (strncmp(buf, "VLIB", 9))
		throw pnm_error();

	pnm_read(file, buf);
	int width = atoi(buf);
	pnm_read(file, buf);
	int height = atoi(buf);

	/* read data */
	*im = new image<T>(width, height);
	file.read((char *)imPtr((*im), 0, 0), width * height * sizeof(T));
}

template <class T>
void save_image(image<T> *im, const char *name) {
	int width = im->width();
	int height = im->height();
	std::ofstream file(name, std::ios::out | std::ios::binary);

	file << "VLIB\n" << width << " " << height << "\n";
	file.write((char *)imPtr(im, 0, 0), width * height * sizeof(T));
}

#endif
