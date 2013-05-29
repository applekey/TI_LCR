#ifndef PROJECTOR_I_PROJECTOR_H_
#define PROJECTOR_I_PROJECTOR_H_

#include <cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class IProjector
{
public:
	virtual bool ProjectImage(cv::Mat image) = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
};

#endif