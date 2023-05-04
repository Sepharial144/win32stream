#ifndef _WIN32_CAPTURE_SCREEN_HPP_
#define _WIN32_CAPTURE_SCREEN_HPP_

#include "CommonTypes.hpp"
#include "ImageAcquisition.hpp"
#include "BasicCaptureScreen.hpp"

class Win32CaptureScreen : public BasicWin32CaptureScreen, public ImageAcquisition
{
public:
	explicit Win32CaptureScreen();
	virtual ~Win32CaptureScreen();

	int32_t nextFrame() override;
	void getFrame(cv::Mat& frame) override;
	void setWindowHandle(HWND handleWindow) override;
	void setRegionOfInterest(const int32_t x, const int32_t y, const int32_t width, const int32_t height) override;
	RectangleScreen rectangle() const override;
	//Image captureImage() override;

private:
	RectangleScreen getRectVirtualScreen();
	void saveToFile(BYTE* data, BITMAPFILEHEADER& m_bfHeader, BITMAPINFOHEADER& m_bitmapHeader);
};


#endif // _WIN32_CAPTURE_SCREEN_HPP_