#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath> 

void wave(const cv::Mat &image, cv::Mat &result) {

    cv::Mat srcX(image.rows, image.cols, CV_32F);
    cv::Mat srcY(image.rows, image.cols, CV_32F);

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            // Les coordonnées X ne changent pas
            srcX.at<float>(i, j) = j;
            // Application d'une transformation sinusoidale sur Y
            srcY.at<float>(i, j) = i + 3 * std::sin(j / 6.0);
        }
    }

    cv::remap(image, result, srcX, srcY, cv::INTER_LINEAR);
}

int main() {
    cv::Mat image = cv::imread("boldt.jpg");
    if (!image.data) {
        return 0;
    }

    cv::Mat result;

    wave(image, result);
    
    cv::namedWindow("Original Image");
    cv::imshow("Original Image", image);

    cv::namedWindow("Wave Effect");
    cv::imshow("Wave Effect", result);

    cv::waitKey(0);

    return 0;
}
