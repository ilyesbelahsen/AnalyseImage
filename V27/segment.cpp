#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "WatershedSegmenter.h"

int main(){
    cv::Mat image = cv::imread("group.jpg");
    if(!image.data)
        return 0;
    
    cv::namedWindow("Original Image");
    cv::imshow("Original Image", image);

    cv::Mat binary;
    binary = cv::imread("binary.bmp",0);

    cv::namedWindow("Binary Image");
    cv::imshow("Binary Image", binary);

    cv::Mat fg;
    cv::erode(binary, fg, cv::Mat(), cv::Point(-1, -1), 4);

    cv::namedWindow("Foreground Image");
    cv::imshow("Foreground Image", fg);

    cv::Mat bg;
    cv::dilate(binary, bg, cv::Mat(), cv::Point(-1, -1), 4);
    cv::threshold(bg, bg, 1, 128, cv::THRESH_BINARY_INV);

    cv::namedWindow("Background Image");
    cv::imshow("Background Image", bg);

    cv::Mat markers(binary.size(), CV_8U, cv::Scalar(0));
    markers = fg + bg;

    cv::namedWindow("Markers");
    cv::imshow("Markers", markers);

    WatershedSegmenter segmenter;

    segmenter.setMarkers(markers);
    segmenter.process(image);

    cv::namedWindow("Segmentation");
    cv::imshow("Segmentation", segmenter.getSegmentation());

    cv::namedWindow("Watersheds");
    cv::imshow("Watersheds", segmenter.getWatersheds());
    
    cv::waitKey(0);
    return 0;
    
}