#include "Exception.hpp"
#include "CommonTypes.hpp"
#include "WinRoutine.hpp"
#include "ProcessEngine.hpp"
#include "CaptureWindow.hpp"
#include "CaptureScreen.hpp"
#include "Image.hpp"

#include <iostream>

#include <opencv2/opencv.hpp>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ INT nCmdShow)
{
    /*
        ProcessEngine::ProcessInternals pIntern;
        pIntern.setProcessName("EVE");
        DWORD pid = pIntern.findProcessId();
        pIntern.readMemory<int>(0x20);
    */

    try
    {

        //RECT windowRect = { 0, 0, 800, 600 };
        //AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
        //HWND handleWindow = CreateDuplicationWindow(windowRect, hInstance, L"ddasample", L"DXGI desktop duplication sample");

        constexpr int32_t cutRight = 16;
        constexpr int32_t cutBottom = 59;

        ProcessEngine::ProcessInternals procIntern{ "Калькулятор" };

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