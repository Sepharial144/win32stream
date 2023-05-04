#include <iostream>
#include "Exception.hpp"
#include "CommonTypes.hpp"
#include "WinRoutine.hpp"
#include "ProcessEngine.hpp"
#include "CaptureWindow.hpp"
#include "CaptureScreen.hpp"
#include "Image.hpp"

#include <opencv2/opencv.hpp>

//int CaptureAnImage(HWND hWnd);
HWND CreateDuplicationWindow(RECT& window_rectangle, HINSTANCE hInstance, const wchar_t* process_name, const wchar_t* window_name);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ INT nCmdShow)
{
    /*
        ProcessEngine::ProcessInternals pIntern;
        pIntern.setProcessName("EVE");
        DWORD pid = pIntern.findProcessId();
        pIntern.readMemory<int>(0x20);
    */


    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    try
    {

        //RECT windowRect = { 0, 0, 800, 600 };
        //AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
        //HWND handleWindow = CreateDuplicationWindow(windowRect, hInstance, L"ddasample", L"DXGI desktop duplication sample");

        constexpr int32_t cutRight = 16;
        constexpr int32_t cutBottom = 59;

        ProcessEngine::ProcessInternals procIntern{ "PUBG: BATTLEGROUNDS" };

        HWND handleWindow = procIntern.findHandleWindow();
        RECT rectWindow = procIntern.getWindowPosition();

        /*
        std::unique_ptr<ImageAcquisition> ptrImageAcqusition = std::make_unique<Win32CaptureWindow>(handleWindow, rectWindow);
        int32_t width  = rectWindow.right - rectWindow.left - cutRight;
        int32_t height = rectWindow.bottom - rectWindow.top - cutBottom;
        ptrImageAcqusition->setRegionOfInterest(0, 0, width, height);
        */

        std::unique_ptr<ImageAcquisition> ptrImageAcqusition = std::make_unique<Win32CaptureScreen>();
        ptrImageAcqusition->setRegionOfInterest(0, 0, 1920, 1080);
        cv::Mat frame;
        while (ptrImageAcqusition->nextFrame())
        {
            ptrImageAcqusition->getFrame(frame);
            cv::imshow("Title", frame);
            cv::waitKey(0);
        }
        //ShowWindow(handleWindow, nCmdShow);
        //UpdateWindow(handleWindow);
    }
    catch (const Win32Exception& e)
    {
        std::cerr << e.what() << &std::endl;
    }

    system("PAUSE");
    return 0;
}

/*
int CaptureAnImage(HWND hWnd)
{
    HDC hdcScreen;
    HDC hdcWindow;
    HDC hdcMemDC = 0;
    HBITMAP hbmScreen = 0;
    BITMAP bmpScreen;
    DWORD dwBytesWritten = 0;
    DWORD dwSizeofDIB = 0;
    HANDLE hFile = 0;
    char* lpbitmap = nullptr;
    HGLOBAL hDIB = 0;
    DWORD dwBmpSize = 0;

    // Retrieve the handle to a display device context for the client
    // area of the window.
    hdcScreen = GetDC(0);
    hdcWindow = GetDC(hWnd);

    // Create a compatible DC, which is used in a BitBlt from the window DC.
    hdcMemDC = CreateCompatibleDC(hdcWindow);

    if (!hdcMemDC)
    {
        MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
        goto done;
    }

    // Get the client area for size calculation.
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);

    // This is the best stretch mode.
    SetStretchBltMode(hdcWindow, HALFTONE);

    // The source DC is the entire screen, and the destination DC is the current window (HWND).
    if (!StretchBlt(hdcWindow,
        0, 0,
        rcClient.right, rcClient.bottom,
        hdcScreen,
        0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        SRCCOPY))
    {
        MessageBox(hWnd, L"StretchBlt has failed", L"Failed", MB_OK);
        goto done;
    }

    // Create a compatible bitmap from the Window DC.
    hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

    if (!hbmScreen)
    {
        MessageBox(hWnd, L"CreateCompatibleBitmap Failed", L"Failed", MB_OK);
        goto done;
    }

    // Select the compatible bitmap into the compatible memory DC.
    SelectObject(hdcMemDC, hbmScreen);

    // Bit block transfer into our compatible memory DC.
    if (!BitBlt(hdcMemDC,
        0, 0,
        rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
        hdcWindow,
        0, 0,
        SRCCOPY))
    {
        MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
        goto done;
    }

    // Get the BITMAP from the HBITMAP.
    GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

    BITMAPFILEHEADER   bmfHeader;
    BITMAPINFOHEADER   bi;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmpScreen.bmWidth;
    bi.biHeight = bmpScreen.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

    // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that
    // call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc
    // have greater overhead than HeapAlloc.
    hDIB = GlobalAlloc(GHND, dwBmpSize);

    lpbitmap = (char*)GlobalLock(hDIB);

    // Gets the "bits" from the bitmap, and copies them into a buffer
    // that's pointed to by lpbitmap.
    GetDIBits(hdcWindow, hbmScreen, 0,
        (UINT)bmpScreen.bmHeight,
        lpbitmap,
        (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    // A file is created, this is where we will save the screen capture.
    hFile = CreateFile(L"captureqwsx.bmp",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);

    // Add the size of the headers to the size of the bitmap to get the total file size.
    dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // Offset to where the actual bitmap bits start.
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    // Size of the file.
    bmfHeader.bfSize = dwSizeofDIB;

    // bfType must always be BM for Bitmaps.
    bmfHeader.bfType = 0x4D42; // BM.

    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

    // Unlock and Free the DIB from the heap.
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);

    // Close the handle for the file that was created.
    CloseHandle(hFile);

    // Clean up.
done:
    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(NULL, hdcScreen);
    ReleaseDC(hWnd, hdcWindow);

    return 0;
}
*/


//
// Window message processor
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_SIZE:
    {
        // Tell output manager that window size has changed
        std::cout << "Resize window" << std::endl;
        //OutMgr.WindowResize();
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}


HWND CreateDuplicationWindow(RECT& window_rectangle, HINSTANCE hInstance, const wchar_t* process_name, const wchar_t* window_name)
{
    HCURSOR Cursor = nullptr;
    WNDCLASSEXW Wc;
    HWND wHandle;

    // Load simple cursor
    Cursor = LoadCursor(nullptr, IDC_ARROW);
    if (!Cursor)
    {
        throw Win32Exception("Cursor load failed");
    }

    // fill window properties
    Wc.cbSize = sizeof(WNDCLASSEXW);
    Wc.style = CS_HREDRAW | CS_VREDRAW;
    Wc.lpfnWndProc = WndProc;
    Wc.cbClsExtra = 0;
    Wc.cbWndExtra = 0;
    Wc.hInstance = hInstance;
    Wc.hIcon = nullptr;
    Wc.hCursor = Cursor;
    Wc.hbrBackground = nullptr;
    Wc.lpszMenuName = nullptr;
    Wc.lpszClassName = L"ddasample";
    Wc.hIconSm = nullptr;
    if (!RegisterClassExW(&Wc))
    {
        throw Win32Exception("Window class registration failed");
    }

    wHandle = CreateWindowW(process_name, window_name,
        WS_OVERLAPPEDWINDOW,
        0, 0,
        window_rectangle.right - window_rectangle.left, window_rectangle.bottom - window_rectangle.top,
        nullptr, nullptr, hInstance, nullptr);

    DestroyCursor(Cursor);

    return wHandle;
}
