#include <iostream>
#include <vector>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>

int main()
{
    cv::Mat image = cv::imread("boldt.jpg", 0);
    if (image.empty())
    {
        return 0;
    }

    cv::Mat sobelX;
    cv::Sobel(image, sobelX, CV_8U, 1, 0, 3, 0.4, 128);
    cv::imshow("Sobel X Image", sobelX);

    cv::Mat sobelY;
    cv::Sobel(image, sobelY, CV_8U, 0, 1, 3, 0.4, 128);
    cv::imshow("Sobel Y Image", sobelY);

    cv::Sobel(image, sobelX, CV_16S, 1, 0);
    cv::Sobel(image, sobelY, CV_16S, 0, 1);

    cv::Mat sobel = abs(sobelX) + abs(sobelY);

    double sobmin, sobmax;
    cv::minMaxLoc(sobel, &sobmin, &sobmax);

    cv::Sobel(image, sobelX, CV_8U, 1, 0, 7, 0.001, 128);
    cv::imshow("Sobel X Image (7x7)", sobelX);

    cv::Mat sobelImage;
    sobel.convertTo(sobelImage, CV_8U, -255.0 / sobmax, 255);
    cv::imshow("Sobel Image", sobelImage);

    cv::Mat sobelThresholded;
    cv::threshold(sobelImage, sobelThresholded, 225, 255, cv::THRESH_BINARY);
    cv::imshow("Binary Sobel Image (low)", sobelThresholded);

    cv::threshold(sobelImage, sobelThresholded, 190, 255, cv::THRESH_BINARY);
    cv::imshow("Binary Sobel Image (high)", sobelThresholded);

    cv::waitKey(0);

    return 0;
}
