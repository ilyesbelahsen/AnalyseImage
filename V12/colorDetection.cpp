#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "colorDetector.h" 

int main()
{
    ColorDetector cdetect;

    cv::Mat image = cv::imread("boldt.jpg");
    if (image.empty()) return 0;

    cdetect.setTargetColor(230, 190, 130);
    cv::Mat result = cdetect.process(image);

    cv::namedWindow("Result", cv::WINDOW_AUTOSIZE);
    cv::imshow("Result", result);

   

    cv::floodFill(image, cv::Point(100, 50), cv::Scalar(255, 255, 255), (cv::Rect*)0, cv::Scalar(35, 35, 35), cv::Scalar(35, 35, 35), cv::FLOODFILL_FIXED_RANGE);
    cv::namedWindow("Flood fill result");
    cv::imshow("Flood fill result", image);

    cv::Mat colors(100, 300, CV_8UC3, cv::Scalar(100, 200, 150));
    cv::Mat range = colors.colRange(0, 100);
    range = range + cv::Scalar(10, 10, 10);
    range = colors.colRange(200, 300);
    range = range + cv::Scalar(-10, -10, -10);

    cv::namedWindow("3 colors");
    cv::imshow("3 colors", colors);

    cv::Mat labImage(100, 300, CV_8UC3, cv::Scalar(100, 200, 150));
    cv::cvtColor(labImage, labImage, cv::COLOR_BGR2Lab);
     cv::waitKey(0);

    return 0;
}
