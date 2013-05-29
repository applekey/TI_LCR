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
	//image = cv::imread("C:\\Users\\song\\Desktop\\convertedBMP\\a.bmp", CV_LOAD_IMAGE_COLOR);
	//imshow("Image", image);
    lcr.ProjectImage("C:\\Users\\song\\Desktop\\convertedBMP\\b.bmp");
	//lcr.ProjectImage("C:\\Users\\song\\Desktop\\convertedBMP\\a.bmp");
	//lcr.ProjectImage("C:\\Users\\song\\Desktop\\convertedBMP\\b.bmp");
	
	return 0;
}