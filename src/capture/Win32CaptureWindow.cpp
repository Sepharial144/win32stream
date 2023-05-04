#include "Win32CaptureWindow.hpp"


Win32CaptureWindow::Win32CaptureWindow(HWND windowHandle, RECT rectWindow)
{
	m_windowHandle = windowHandle;
	Win32CaptureChecker::checkWin32ValueMessage<HWND>(m_windowHandle, "Win32CaptureWindow device context of window is nullptr");

	m_bitmapWidth  = rectWindow.right - rectWindow.left;
	m_bitmapHeight = rectWindow.bottom - rectWindow.top;

	const WORD bitsPerPixel = 24;
	getBitmapHeader(m_bfHeader, m_bitmapHeader, bitsPerPixel, m_bitmapWidth, m_bitmapHeight * (-1));
	m_bitmapInfo.bmiHeader = m_bitmapHeader;

	setRegionOfInterest(0, 0, m_bitmapWidth, m_bitmapHeight);
	m_stride = computeStride(m_bitmapInfo.bmiHeader.biBitCount, m_bitmapWidth);
}


Win32CaptureWindow::~Win32CaptureWindow()
{
}


void Win32CaptureWindow::setWindowHandle(HWND handleWindow)
{
	m_windowHandle = handleWindow;
}


void Win32CaptureWindow::setRegionOfInterest(const int32_t x, const int32_t y, const int32_t width, const int32_t height)
{
	m_regionInterest = RegionInterest{ x, y, width, height };
}


RectangleScreen Win32CaptureWindow::rectangle() const
{
	return RectangleScreen{ m_regionInterest.x, m_regionInterest.y, m_regionInterest.width, m_regionInterest.height };
}


int32_t Win32CaptureWindow::nextFrame()
{
	if (!m_windowHandle)
	{
		throw InvalidHandleException("Capture window: invalid value of handle");
		return 0;
	}

	cleanFrameInfo(m_windowHandle);
	try
	{
		captureDeviceContext(m_frameInfo.hdc, m_windowHandle);
		Win32CaptureChecker::checkWin32ValueMessage<HDC>(m_frameInfo.hdc, "Win32CaptureWindow window device context handle is null");

		createCompatibleDeviceContext(m_frameInfo.hdcMemory, m_frameInfo.hdc);
		Win32CaptureChecker::checkWin32ValueMessage<HDC>(m_frameInfo.hdcMemory, "Win32CaptureWindow memory handle is null");

		createDeviceIndependentSection(m_frameInfo.handleBitmap, &m_frameInfo.data, m_frameInfo.hdc);
		Win32CaptureChecker::checkWin32ValueMessage<HBITMAP>(m_frameInfo.handleBitmap, "Win32CaptureWindow bitmap handle is null");

		selectGDIObject(m_frameInfo.handleGdiObj, m_frameInfo.hdcMemory, m_frameInfo.handleBitmap);
		Win32CaptureChecker::checkWin32ValueMessage<HGDIOBJ>(m_frameInfo.handleGdiObj, "Win32CaptureWindow gdi object handle is null");

		m_frameInfo.status = 1;
	}
	catch (const Win32Exception& e)
	{
		std::cout << "Error to process frame: " << e.what() << &std::endl;
		m_frameInfo.status = -1;
	}

	return m_frameInfo.status;
}


void Win32CaptureWindow::getFrame(cv::Mat& frame)
{
	if (m_frameInfo.status)
	{
		BitBlt(m_frameInfo.hdcMemory, m_regionInterest.x, m_regionInterest.y, m_regionInterest.width, m_regionInterest.height, m_frameInfo.hdc, 0, 0, SRCCOPY);
		frame = std::move(cv::Mat{ m_regionInterest.height, m_regionInterest.width, CV_8UC3, m_frameInfo.data, m_stride });
	}
}


/*
Image Win32CaptureWindow::captureImage()
{
	if (!m_windowHandle)
	{
		std::cout << "Win32CaptureWindow: handle of windows is null" << &std::endl;
		return Image{StatusImage::Bad};
	}

	HDC hdc = GetDC(m_windowHandle);
	HDC hdcMemory = CreateCompatibleDC(hdc);

	void* pData = nullptr;
	HBITMAP handleBitmap = CreateDIBSection(hdc, &m_bitmapInfo, DIB_RGB_COLORS, &pData, nullptr, 0);

	if (!handleBitmap)
	{
		std::cout << "Win32CaptureWindow: handleBitmap is null" << &std::endl;
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


void Win32CaptureWindow::saveToFile(BYTE* data, BITMAPFILEHEADER& m_bfHeader, BITMAPINFOHEADER& m_bitmapHeader)
{
	DWORD dwWritten = 0;
	DWORD cbBytes = computeBytes(m_bitmapHeader.biBitCount, m_bitmapHeader.biWidth, m_bitmapHeader.biHeight);
	HANDLE hFile = CreateFileW(L"captureqwsx.bmp", GENERIC_WRITE | GENERIC_READ, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	WriteFile(hFile, &m_bfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, nullptr);
	WriteFile(hFile, &m_bitmapHeader, sizeof(BITMAPINFOHEADER), &dwWritten, nullptr);
	WriteFile(hFile, data, cbBytes, &dwWritten, nullptr);

	CloseHandle(hFile);
}