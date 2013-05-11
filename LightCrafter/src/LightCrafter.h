#ifndef _LIGHTCRAFTER_H_
#define _LIGHTCRAFTER_H_

using namespace std;
//#include <cv.h> 
#include <string>
#include "LCR_Commander.h"
#include "LCR_Common.h"
#include <iostream>

#include "IProjector.h"



//#define LCR_Default_IP			"192.168.1.100"
//#define LCR_Default_PORT		"21845"

//testing purposes 

#define LCR_Default_IP			"127.0.0.1"
#define LCR_Default_PORT		"1234"

class LightCrafter: public IProjector
{
private:
	bool IsConnected;
	LCR_Commander*  Commander;
	unique_ptr<uint8[]> _Convert2BinaryImage(cv::Mat byteImage);

	static const uint8 MSB_HIGH  = 0x80;
    static const uint8 MSB_LOW	  = 0x7F;


public:
	LightCrafter(void);
	~LightCrafter(void);

	//int start();
	//void stop();

	//upload a static image, static image has to be a bmp at 608 x 684
	bool ProjectImage(cv::Mat image);

	int LightCrafter::GetHeight(void);

	int LightCrafter::GetWidth(void);
};

#endif