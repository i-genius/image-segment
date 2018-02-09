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

#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "pnmfile.h"

#define BUF_SIZE 256  //2^8

void read_packed(unsigned char *data, int size, std::ifstream &f)
{
	unsigned char uc_ch = 0;
	int n_bitshift = -1;

	for (int i = 0; i < size; i++) {

		if (n_bitshift == -1) {
			uc_ch = f.get();
			n_bitshift = 7;
		}

		data[i] = (uc_ch >> n_bitshift) & 1;
		n_bitshift--;
	}
}

void write_packed(unsigned char *data, int size, std::ofstream &f)
{
	unsigned char uc_ch = 0;
	int n_bitshift = 7;

	for (int i = 0; i < size; i++) {
		uc_ch = uc_ch | (data[i] << n_bitshift);
		n_bitshift--;

		if ((n_bitshift == -1) || (i == size - 1)) {
			f.put(uc_ch);
			n_bitshift = 7;
			uc_ch = 0;
		}
	}
}

/* read PNM field, skipping comments */
void pnm_read(std::ifstream &file, char *buf)
{
	char ac_doc[BUF_SIZE];
	char ch;

	file >> ch;

	while (ch == '#') {
		file.getline(ac_doc, BUF_SIZE);
		file >> ch;
	}

	file.putback(ch);

	file.width(BUF_SIZE);
	file >> buf;
	file.ignore();
}

image<uchar> *loadPBM(const char *name)
{
	char ac_buf[BUF_SIZE];

	/* read header */
	std::ifstream o_in_file(name, std::ios::in | std::ios::binary);
	pnm_read(o_in_file, ac_buf);

	if (strncmp(ac_buf, "P4", 2)) {
		throw pnm_error();
	}

	pnm_read(o_in_file, ac_buf);
	int i_width = atoi(ac_buf);
	pnm_read(o_in_file, ac_buf);
	int i_height = atoi(ac_buf);

	/* read data */
	image<uchar> *pouc_img = new image<uchar>(i_width, i_height);

	for (int i = 0; i < i_height; i++) {
		read_packed(imPtr(pouc_img, 0, i), i_width, o_in_file);
	}

	return pouc_img;
}

void savePBM(image<uchar> *im, const char *name)
{
	int i_width = im->width();
	int i_height = im->height();
	std::ofstream o_out_file(name, std::ios::out | std::ios::binary);

	o_out_file << "P4\n" << i_width << " " << i_height << "\n";

	for (int i = 0; i < i_height; i++) {
		write_packed(imPtr(im, 0, i), i_width, o_out_file);
	}
}

image<uchar> *loadPGM(const char *name)
{
	char ac_buf[BUF_SIZE];

	/* read header */
	std::ifstream o_in_file(name, std::ios::in | std::ios::binary);
	pnm_read(o_in_file, ac_buf);
	if (strncmp(ac_buf, "P5", 2))
		throw pnm_error();

	pnm_read(o_in_file, ac_buf);
	int i_width = atoi(ac_buf);
	pnm_read(o_in_file, ac_buf);
	int i_height = atoi(ac_buf);

	pnm_read(o_in_file, ac_buf);

	if (atoi(ac_buf) > UCHAR_MAX) {
		throw pnm_error();
	}

	/* read data */
	image<uchar> *pouc_img = new image<uchar>(i_width, i_height);
	o_in_file.read((char *)imPtr(pouc_img, 0, 0), i_width * i_height * sizeof(uchar));

	return pouc_img;
}

void savePGM(image<uchar> *im, const char *name)
{
	int i_width = im->width();
	int i_height = im->height();
	std::ofstream o_out_file(name, std::ios::out | std::ios::binary);

	o_out_file << "P5\n" << i_width << " " << i_height << "\n" << UCHAR_MAX << "\n";

	o_out_file.write((char *)imPtr(im, 0, 0), i_width * i_height * sizeof(uchar));
}

image<rgb> *loadPPM(const char *name)
{
	char ac_buf[BUF_SIZE];

	/* read header */
	std::ifstream o_in_file(name, std::ios::in | std::ios::binary);
	pnm_read(o_in_file, ac_buf);

	if (strncmp(ac_buf, "P6", 2)) {
		throw pnm_error();
	}

	pnm_read(o_in_file, ac_buf);
	int i_width = atoi(ac_buf);
	pnm_read(o_in_file, ac_buf);
	int i_height = atoi(ac_buf);

	pnm_read(o_in_file, ac_buf);

	if (atoi(ac_buf) > UCHAR_MAX) {
		throw pnm_error();
	}

	/* read data */
	image<rgb> *po_img = new image<rgb>(i_width, i_height);
	o_in_file.read((char *)imPtr(po_img, 0, 0), i_width * i_height * sizeof(rgb));

	return po_img;
}

void savePPM(image<rgb> *im, const char *name)
{
	int i_width = im->width();
	int i_height = im->height();
	std::ofstream o_out_file(name, std::ios::out | std::ios::binary);

	o_out_file << "P6\n" << i_width << " " << i_height << "\n" << UCHAR_MAX << "\n";

	o_out_file.write((char *)imPtr(im, 0, 0), i_width * i_height * sizeof(rgb));
}
