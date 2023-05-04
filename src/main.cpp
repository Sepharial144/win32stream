#include "win32stream.hpp"

#include <iostream>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

//int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ INT nCmdShow)
int main(int argc, char* argv[])
{
    std::unique_ptr<ImageAcquisition> ptrImageAcqusition;

    try
    {
        std::unique_ptr<ImageAcquisition> ptrImageAcqusition = std::make_unique<Win32CaptureScreen>();
        ptrImageAcqusition->setRegionOfInterest(0, 0, 1920, 1080);

        cv::Mat frame;
        while (ptrImageAcqusition->nextFrame())
        {
            ptrImageAcqusition->getFrame(frame);
            cv::imshow("ScreenCaptureExample", frame);
            cv::waitKey(0);
        }
    }
    catch (const Win32Exception& e)
    {
        std::cerr << e.what() << &std::endl;
    }

    return 0;
}