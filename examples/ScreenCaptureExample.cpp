#include <iostream>
#include "Exception.hpp"
#include "CommonTypes.hpp"
#include "WinRoutine.hpp"
#include "ProcessEngine.hpp"
#include "CaptureWindow.hpp"
#include "CaptureScreen.hpp"
#include "Image.hpp"

#include <opencv2/opencv.hpp>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ INT nCmdShow)
{
    std::unique_ptr<ImageAcquisition> ptrImageAcqusition;

    try
    {
        ptrImageAcqusition = std::make_unique<Win32CaptureScreen>();
        ptrImageAcqusition->setRegionOfInterest(0, 0, 1920, 1080);
    }
    catch (const Win32Exception& e)
    {
        std::cerr << e.what() << &std::endl;
    }

    cv::Mat frame;
    while (ptrImageAcqusition->nextFrame())
    {
        ptrImageAcqusition->getFrame(frame);
        cv::imshow("ScreenCaptureExample", frame);
        cv::waitKey(0);
    }

    return 0;
}