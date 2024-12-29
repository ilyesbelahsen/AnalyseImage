#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "../V16_17_18/histogram.h" 
#include "contentFinder.h" 

int main()
{
    cv::Mat image = cv::imread("waves.jpg", 0);
    if (!image.data)
        return 0;

    cv::Mat imageROI;
    imageROI = image(cv::Rect(216, 33, 24, 30));

    Histogram1D h;
    cv::Mat hist = h.getHistogram(imageROI);

    ContentFinder finder;
    finder.setHistogram(hist);
    finder.setThreshold(-1.0f);

    cv::Mat result1;
    result1 = finder.find(image);

    cv::Mat tmp;
    result1.convertTo(tmp, CV_8U, -1.0, 255.0);
    cv::namedWindow("Backprojection result");
    cv::imshow("Backprojection result", tmp);

    finder.setThreshold(0.12f);
    result1 = finder.find(image);

    cv::rectangle(image, cv::Rect(216, 33, 24, 30), cv::Scalar(0, 0, 0));

    cv::namedWindow("Image");
    cv::imshow("Image", image);

    cv::namedWindow("Detection Result");
    cv::imshow("Detection Result", result1);

    // ColorHistogram hc;
    // cv::Mat color = cv::imread("waves.jpg");

    // imageROI = color(cv::Rect(0, 0, 100, 45));

    // hc.setSize(8);
    // cv::Mat shist = hc.getHistogram(imageROI);

    // finder.setHistogram(shist);
    // finder.setThreshold(0.05f);

    // result1 = finder.find(color);

    // cv::namedWindow("Color Detection Result");
    // cv::imshow("Color Detection Result", result1);


    return 0;
}
