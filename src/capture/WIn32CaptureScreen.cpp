#include "Win32CaptureScreen.hpp"
#include <utility>


Win32CaptureScreen::Win32CaptureScreen()
{
	m_windowHandle = GetDesktopWindow();
	Win32CaptureChecker::checkWin32ValueMessage<HWND>(m_windowHandle, "Win32CaptureScreen desktop handle is null");

	RectangleScreen rectScreen = getRectVirtualScreen();
	m_bitmapWidth  = rectScreen.width;
	m_bitmapHeight = rectScreen.height;

	const WORD bitsPerPixel = 24;
	getBitmapHeader(m_bfHeader, m_bitmapHeader, bitsPerPixel, m_bitmapWidth, m_bitmapHeight * (-1));
	m_bitmapInfo.bmiHeader = m_bitmapHeader;

	setRegionOfInterest(0, 0, m_bitmapWidth, m_bitmapHeight);

	m_stride = computeStride(m_bitmapInfo.bmiHeader.biBitCount, m_bitmapWidth);
}


Win32CaptureScreen::~Win32CaptureScreen()
{
}


void Win32CaptureScreen::setWindowHandle(HWND handleWindow)
{
	m_windowHandle = handleWindow;
}


void Win32CaptureScreen::setRegionOfInterest(const int32_t x, const int32_t y, const int32_t width, const int32_t height)
{
	m_regionInterest = RegionInterest{ x, y, width, height };
}

RectangleScreen Win32CaptureScreen::getRectVirtualScreen()
{
	RectangleScreen rect;
	rect.x      = GetSystemMetrics(SM_XVIRTUALSCREEN);
	rect.y      = GetSystemMetrics(SM_YVIRTUALSCREEN);
	rect.width  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	rect.height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	return rect;
}

RectangleScreen Win32CaptureScreen::rectangle() const
{
	return RectangleScreen{ m_regionInterest.x, m_regionInterest.y, m_regionInterest.width, m_regionInterest.height };
}


int32_t Win32CaptureScreen::nextFrame()
{

	if (!m_windowHandle)
	{
		throw InvalidHandleException("Win32CaptureScreen: invalid value of handle");
		return 0;
	}

	cleanFrameInfo(m_windowHandle);
	try
	{
		captureDeviceContext(m_frameInfo.hdc, m_windowHandle);
		Win32CaptureChecker::checkWin32ValueMessage<HDC>(m_frameInfo.hdc, "Win32CaptureScreen desktop device context handle is null");

		createCompatibleDeviceContext(m_frameInfo.hdcMemory, m_frameInfo.hdc);
		Win32CaptureChecker::checkWin32ValueMessage<HDC>(m_frameInfo.hdcMemory, "Win32CaptureScreen memory handle is null");

		createDeviceIndependentSection(m_frameInfo.handleBitmap, &m_frameInfo.data, m_frameInfo.hdc);
		Win32CaptureChecker::checkWin32ValueMessage<HBITMAP>(m_frameInfo.handleBitmap, "Win32CaptureScreen bitmap handle is null");

		selectGDIObject(m_frameInfo.handleGdiObj, m_frameInfo.hdcMemory, m_frameInfo.handleBitmap);
		Win32CaptureChecker::checkWin32ValueMessage<HGDIOBJ>(m_frameInfo.handleGdiObj, "Win32CaptureScreen gdi object handle is null");

		m_frameInfo.status = 1;
	}
	catch (const Win32Exception& e)
	{
		std::cout << "Error to process frame: " << e.what() << &std::endl;
		m_frameInfo.status = -1;
	}

	return m_frameInfo.status;
}


void Win32CaptureScreen::getFrame(cv::Mat& frame)
{
	if (m_frameInfo.status)
	{
		BitBlt(m_frameInfo.hdcMemory, m_regionInterest.x, m_regionInterest.y, m_regionInterest.width, m_regionInterest.height, m_frameInfo.hdc, 0, 0, SRCCOPY);
		frame = std::move(cv::Mat{ m_regionInterest.height, m_regionInterest.width, CV_8UC3, m_frameInfo.data, m_stride });
	}
}


/*
Image Win32CaptureScreen::captureImage()
{
	if (!m_windowHandle)
	{
		std::cout << "Win32CaptureScreen: handle of windows is null" << &std::endl;
		return Image{ StatusImage::Bad };
	}

	HDC hdc = GetDC(m_windowHandle);
	HDC hdcMemory = CreateCompatibleDC(hdc);

	void* pData = nullptr;
	HBITMAP handleBitmap = CreateDIBSection(hdc, &m_bitmapInfo, DIB_RGB_COLORS, &pData, nullptr, 0);

	if (!handleBitmap)
	{
		std::cout << "Win32CaptureScreen: handleBitmap is null" << &std::endl;
		return Image{ StatusImage::Bad };
	}

	SelectObject(hdcMemory, handleBitmap);
	Image img{ m_regionInterest.width, m_regionInterest.height, CV_8UC3, pData, m_stride };

	BitBlt(hdcMemory, m_regionInterest.x, m_regionInterest.y, m_regionInterest.width, m_regionInterest.height, hdc, 0, 0, SRCCOPY);

	DeleteDC(hdcMemory);
	DeleteObject(handleBitmap);
	ReleaseDC(m_windowHandle, hdc);

	return img;
}
*/


void Win32CaptureScreen::saveToFile(BYTE* data, BITMAPFILEHEADER& m_bfHeader, BITMAPINFOHEADER& m_bitmapHeader)
{
	DWORD dwWritten = 0;
	DWORD cbBytes = computeBytes(m_bitmapHeader.biBitCount, m_bitmapHeader.biWidth, m_bitmapHeader.biHeight);
	HANDLE hFile = CreateFileW(L"captureqwsx.bmp", GENERIC_WRITE | GENERIC_READ, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	WriteFile(hFile, &m_bfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, nullptr);
	WriteFile(hFile, &m_bitmapHeader, sizeof(BITMAPINFOHEADER), &dwWritten, nullptr);
	WriteFile(hFile, data, cbBytes, &dwWritten, nullptr);

	CloseHandle(hFile);
}