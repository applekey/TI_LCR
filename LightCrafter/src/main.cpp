#include <stdio.h>
#include "cv.h"

#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "LightCrafter.h"

using namespace std;
using namespace cv;

int main()
{
	LightCrafter lcr;
	//lcr.start();
	

	cv::Mat image;
	image = cv::imread("C:\\Users\\song\\Desktop\\rsz_lightcrafter_canada_converted.bmp", CV_LOAD_IMAGE_COLOR);
	//imshow("Image", image);
    lcr.ProjectImage(image);
	
	return 0;
}