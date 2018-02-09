/*
* @File: test.cpp
* @Author: tangxuan
*
* @Created on April 20, 2017, 8:06 AM
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

#include "generate_bound.h"
#include "segment_image.h"

#include "convert.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
	const char image_name[100] = "../../data/src2.png";
	const char out[100] = "../../data/result/bound_coordinate.txt";

	float sigma = 0.8f;
	float k = 200;
	int min_size = 100;

	// read image
	cv::Mat img = cv::imread(image_name);

	// convert to image type
	image<rgb> *input = mat2Image(img);

	int region_nums;
	int *bound = generate_bounding_boxes(input, sigma, k, min_size, &region_nums);

	FILE* f;
	f = fopen(out, "w");

	for (int i = 0; i < region_nums; i++) {
		fprintf(f, "%d\t%d\t%d\t%d \n", bound[4 * i + 0], bound[4 * i + 1], bound[4 * i + 2], bound[4 * i + 3]);
                //fprintf(f, "%d\t%d\t%d\t%d \n", bound[4 * i + 1], bound[4 * i + 0], bound[4 * i + 3], bound[4 * i + 2]);
        }

	fclose(f);

	for (int i = 0; i < region_nums; i++){
		rgb color = random_rgb();
		for (int j = bound[4 * i + 0]; j <= bound[4 * i + 2]; j++){  // from xmin to xmax, set color to point where ymin to ymax
			imRef(input, j, bound[4 * i + 1]) = color;
                        //imRef(input, j, bound[4 * i + 1] + 1) = color;    //add the box line's line_width
                        //imRef(input, j, bound[4 * i + 1] + 2) = color;
                        //imRef(input, j, bound[4 * i + 1] + 3) = color;
                        
			imRef(input, j, bound[4 * i + 3]) = color;
                        //imRef(input, j, bound[4 * i + 3] - 1) = color;
                        //imRef(input, j, bound[4 * i + 3] - 2) = color;
                        //imRef(input, j, bound[4 * i + 3] - 3) = color;
		}
		for (int j = bound[4 * i + 1]; j <= bound[4 * i + 3]; j++){  // from ymin to ymax, set color to point where xmin to xmax
			imRef(input, bound[4 * i + 0], j) = color;
                        //imRef(input, bound[4 * i + 0] + 1, j) = color;    //add the box line's line_width
                        //imRef(input, bound[4 * i + 0] + 2, j) = color;
                        //imRef(input, bound[4 * i + 0] + 3, j) = color;
                        
			imRef(input, bound[4 * i + 2], j) = color;
			//imRef(input, bound[4 * i + 2] - 1, j) = color;
			//imRef(input, bound[4 * i + 2] - 2, j) = color;
			//imRef(input, bound[4 * i + 2] - 3, j) = color;                        
		}
	}

	img = *image2Mat(input);
	cv::imwrite("../../data/result/res.png", img);
	printf("OK.\n");

	delete input;
	delete bound;
	return 0;
}

