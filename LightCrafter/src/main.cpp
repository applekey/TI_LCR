#include <stdio.h>
#include "cv.h"

#include <iostream>

#include "LightCrafter.h"
#include "BitmapCreator.h"

using namespace std;
using namespace cv;

int main()
{

	cv::Mat image1;
	image1 = cv::imread("C:\\Users\\song\\Desktop\\convertedBMP\\b.bmp", CV_LOAD_IMAGE_UNCHANGED );

	cv::Mat image2;
	image2 = cv::imread("C:\\Users\\song\\Desktop\\convertedBMP\\a.bmp", CV_LOAD_IMAGE_UNCHANGED );

	
	
	LightCrafter lcr;
	lcr.Connect();

	//lcr.StaticDisplayMode();

	for(int i =0;i<60;i++)
	{
	 lcr.ProjectImage(image2);
	 lcr.ProjectImage(image1);
	}
	


	lcr.Disconnect();
	return 0;
}