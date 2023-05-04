#ifndef _BASIC_WIN32_CAPTURE_SCREEN_HPP_
#define _BASIC_WIN32_CAPTURE_SCREEN_HPP_

#include "Exception/CaptureChecker.hpp"
#include "CommonTypes.hpp"


class BasicWin32CaptureScreen: public Win32CaptureChecker
{
public:
	explicit BasicWin32CaptureScreen();
	virtual ~BasicWin32CaptureScreen();

protected:
	void captureDeviceContext(HDC& dest_hdc, const HWND handle_window);
	void createCompatibleDeviceContext(HDC& dest_hdc, const HDC handle_dc);
	void createCompatibleBitmap(HBITMAP& handle_bitmap, HDC src_hdc, int32_t cx, int32_t cy);
	void createDeviceIndependentSection(HBITMAP& handle_bitmap, void** pData, HDC src_hdc);
	void selectGDIObject(HGDIOBJ& handle_gdi_obj, HDC handle_dc, HGDIOBJ handle_object);
	void cleanFrameInfo(HWND handle_window);

	void getBitmapHeader(BITMAPFILEHEADER& bFileHeader, BITMAPINFOHEADER& bHeaderInfo, WORD bit_count, int32_t width, int32_t height);
	DWORD computeStride(WORD bitsPerPixel, uint32_t width) noexcept;
	DWORD computeBytes(WORD bitsPerPixel, uint32_t width, uint32_t height) noexcept;


protected:
	HWND m_windowHandle;
	FrameInfo m_frameInfo;
	RegionInterest m_regionInterest;
	int32_t m_bitmapWidth;
	int32_t m_bitmapHeight;
	size_t m_stride;
	BITMAPFILEHEADER m_bfHeader;
	BITMAPINFOHEADER m_bitmapHeader;
	BITMAPINFO m_bitmapInfo;
	BITMAP m_bitmapWindow;
};

#endif // _BASIC_WIN32_CAPTURE_SCREEN_HPP_

