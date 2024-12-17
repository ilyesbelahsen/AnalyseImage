#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::namedWindow("Image");
    cv::Mat image = cv::imread("images/puppy.bmp");
    cv::Mat logo = cv::imread("images/smalllogo.png");

    cv::Mat imageROI(image, cv::Rect(image.cols-logo.cols, image.rows-image.rows, logo.cols, logo.rows));

    logo.copyTo(imageROI);

    cv::imshow("Imagess", image);

  cv::waitKey(0);
    image = cv::imread("images/puppy.bmp");

    imageROI= image(cv::Rect(image.cols-logo.cols,image.rows-logo.rows, logo.cols, logo.rows));

    // imageROI = image(cv::Range(image.rows-logo.rows, image.rows),
    //                     cv::Range(image.cols-logo.cols, image.cols));

    // cv::Mat imageROI = image.rowRange(start, end);
    // cv::Mat imageROI = image.colRange(start, end);

    cv::Mat mask(logo);

    logo.copyTo(imageROI, mask);
    cv::imshow("Image", image);
    cv::waitKey(0);
    return 0;
}