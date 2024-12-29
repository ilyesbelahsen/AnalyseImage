#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "colorhistogram.h"
#include "../V19/contentFinder.h"

int main() {
    cv::Mat image = cv::imread("baboon01.jpg");
    if (!image.data)
        return 0;

    cv::Rect rect(110, 45, 35, 45);
    cv::rectangle(image, rect, cv::Scalar(0, 0, 255));

    cv::Mat imageROI = image(rect);

    cv::namedWindow("Image 1");
    cv::imshow("Image 1", image);

    int minSat = 65;
    ColorHistogram hc;
    cv::Mat colorhist = hc.getHueHistogram(imageROI, minSat);

    ContentFinder finder;
    finder.setHistogram(colorhist);
    finder.setThreshold(0.2f);

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    std::vector<cv::Mat> v;
    cv::split(hsv, v);

    cv::threshold(v[1], v[1], minSat, 255, cv::THRESH_BINARY);

    image = cv::imread("baboon02.jpg");

    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    int ch[1] = {0};
    finder.setThreshold(-1.0f);
    cv::Mat result = finder.find(hsv, 0.0f, 180.0f, ch);

    cv::rectangle(image, rect, cv::Scalar(0, 0, 255));

    cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 10, 1);
    std::cout << "meanshift = " << cv::meanShift(result, rect, criteria) << std::endl;

    cv::rectangle(image, rect, cv::Scalar(0, 255, 0));
    cv::namedWindow("Image 2 result");
    cv::imshow("Image 2 result", image);

    cv::waitKey();
    return 0;
}
