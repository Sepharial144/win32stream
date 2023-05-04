// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#ifndef _COMMONTYPES_HPP_
#define _COMMONTYPES_HPP_

#include <windows.h>
//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "DirectXTK.lib")
//#include <d3d11.h>
//#include <dxgi1_2.h>
//#include <sal.h>
//#include <new>
//#include <warning.h>
//#include <DirectXMath.h>

#include <opencv2/opencv.hpp>

struct FrameInfo
{
	int32_t status;
	HDC hdc;
	HDC hdcMemory;
	HBITMAP handleBitmap;
	void* data;
	HGDIOBJ handleGdiObj;
};

struct RegionInterest
{
	int32_t x;
	int32_t y;
	int32_t width;
	int32_t height;
};

struct WindowInfo
{
	HWND handleWindow;
	RECT rectangleWindow;
};


struct RectangleScreen
{
	int32_t x;
	int32_t y;
	int32_t width;
	int32_t height;
};

enum class StatusImage
{
	Bad,
	Good
};


#endif // _COMMONTYPES_HPP_
