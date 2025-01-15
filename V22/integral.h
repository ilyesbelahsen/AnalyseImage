#ifndef INTEGRAL_H
#define INTEGRAL_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <cmath>

template<typename T, int N>
class IntegralImage {
private:
    cv::Mat integralImage; 

public:
    IntegralImage(const cv::Mat& image) { 
        cv::integral(image, integralImage, cv::DataType<T>::type); 
    }

    cv::Vec<T, N> operator()(int xo, int yo, int width, int height) const {
        return integralImage.at<cv::Vec<T, N>>(yo + height, xo + width)
             - integralImage.at<cv::Vec<T, N>>(yo + height, xo)
             - integralImage.at<cv::Vec<T, N>>(yo, xo + width)
             + integralImage.at<cv::Vec<T, N>>(yo, xo);
    }

    // cv::Vec<T, N> operator()(int x, int y, int radius) const {
    //     return integralImage(y + radius + 1, x + radius + 1)
    //          - integralImage(y + radius + 1, x - radius)
    //          - integralImage(y - radius + 1, x - radius)
    //          + integralImage(y - radius, x - radius);
    // }

    void convertTobinaryPlanes(const cv::Mat& input, cv::Mat& output, int nPlanes) {
        int n = 8 - static_cast<int>(std::log(static_cast<double>(nPlanes)) / std::log(2.0));
        uchar mask = 0xFF << n;

        std::vector<cv::Mat> planes;
        cv::Mat reduced = input & mask;
        
        for (int i = 0; i < nPlanes; i++) {
            cv::Mat plane = (reduced == (i << n));
            planes.push_back(plane);
        }

        cv::merge(planes, output);
    }
};

#endif 
