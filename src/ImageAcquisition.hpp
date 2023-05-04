#ifndef _WIN32STREAM_I_IMAGE_ACQUISITION_HPP_
#define _WIN32STREAM_I_IMAGE_ACQUISITION_HPP_

#include "capture/CommonTypes.hpp"

#include <memory>

class ImageAcquisition
{
public:
	ImageAcquisition() {};
	virtual ~ImageAcquisition() {};
	virtual int32_t nextFrame() = 0;
	virtual void getFrame(cv::Mat& frame) = 0;
	virtual void setWindowHandle(HWND handleWindow) = 0;
	virtual void setRegionOfInterest(const int32_t x, const int32_t y, const int32_t width, const int32_t height) = 0;
	virtual RectangleScreen rectangle() const = 0;
};


#endif // _WIN32STREAM_I_IMAGE_ACQUISITION_HPP_
