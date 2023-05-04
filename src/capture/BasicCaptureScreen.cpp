#include "BasicCaptureScreen.hpp"


BasicWin32CaptureScreen::BasicWin32CaptureScreen()
	:m_windowHandle{ nullptr },
	 m_frameInfo{},
	 m_regionInterest{},
	 m_bitmapWidth{},
	 m_bfHeader{},
	 m_stride{},
	 m_bitmapHeader{},
	 m_bitmapInfo{},
	 m_bitmapWindow{}
{
	ZeroMemory(&m_frameInfo, sizeof(FrameInfo));
	ZeroMemory(&m_regionInterest, sizeof(RegionInterest));
	ZeroMemory(&m_bfHeader, sizeof(BITMAPFILEHEADER));
	ZeroMemory(&m_bitmapHeader, sizeof(BITMAPINFOHEADER));
	ZeroMemory(&m_bitmapInfo, sizeof(BITMAPINFO));
	ZeroMemory(&m_bitmapWindow, sizeof(BITMAP));
}


BasicWin32CaptureScreen::~BasicWin32CaptureScreen()
{
	if (m_frameInfo.status != 0)
	{
		cleanFrameInfo(m_windowHandle);
	}
}


void BasicWin32CaptureScreen::captureDeviceContext(HDC& dest_hdc, const HWND handle_window)
{
	dest_hdc = GetDC(handle_window);
}


void BasicWin32CaptureScreen::createCompatibleDeviceContext(HDC& dest_hdc, const HDC src_hdc)
{
	dest_hdc = CreateCompatibleDC(src_hdc);
}

void BasicWin32CaptureScreen::createCompatibleBitmap(HBITMAP& handle_bitmap, HDC src_hdc, int32_t cx, int32_t cy)
{
	handle_bitmap = CreateCompatibleBitmap(src_hdc, cx, cy);
}


void BasicWin32CaptureScreen::createDeviceIndependentSection(HBITMAP& handle_bitmap, void** pData, HDC src_hdc)
{
	handle_bitmap = CreateDIBSection(src_hdc, &m_bitmapInfo, DIB_RGB_COLORS, pData, nullptr, 0);
}


void BasicWin32CaptureScreen::selectGDIObject(HGDIOBJ& handle_gdi_obj, HDC handle_dc, HGDIOBJ handle_object)
{
	handle_gdi_obj = SelectObject(handle_dc, handle_object);
}


void BasicWin32CaptureScreen::cleanFrameInfo(HWND handle_window)
{
	ReleaseDC(handle_window, m_frameInfo.hdc);
	DeleteDC(m_frameInfo.hdcMemory);
	DeleteObject(m_frameInfo.handleBitmap);
	ZeroMemory(&m_frameInfo, sizeof(FrameInfo));
}


DWORD BasicWin32CaptureScreen::computeStride(WORD bitsPerPixel, uint32_t width) noexcept
{
	return ((width * bitsPerPixel + 31) / 32) * 4;
}


DWORD BasicWin32CaptureScreen::computeBytes(WORD bitsPerPixel, uint32_t width, uint32_t height) noexcept
{
	DWORD stride = computeStride(bitsPerPixel, width);
	return height * stride;
}


void BasicWin32CaptureScreen::getBitmapHeader(BITMAPFILEHEADER& bFileHeader, BITMAPINFOHEADER& bHeaderInfo, WORD bit_count, int32_t width, int32_t height)
{
	bFileHeader.bfType = (WORD)('B' | ('M' << 8));
	bFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bHeaderInfo.biSize = sizeof(BITMAPINFOHEADER);
	bHeaderInfo.biBitCount = bit_count;
	bHeaderInfo.biCompression = BI_RGB;
	bHeaderInfo.biPlanes = 1;
	bHeaderInfo.biWidth = width;
	bHeaderInfo.biHeight = height;
}