#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "integral.h"


int main() {
    cv::Mat image = cv::imread("book.jpg", 0);
    if(!image.data)
        return 0;
    cv::transpose(image,image);
    cv::flip(image,image,0);
    cv::namedWindow("Original Image");
    cv::imshow("Original Image", image);
    cv::Mat binaryFixed;
    cv::threshold(image, binaryFixed, 70,255, cv::THRESH_BINARY);

    cv::Mat iimage;
    cv::integral(image, iimage, CV_32S);

    int blockSize = 21;
    int threshold = 10;

    int halfSize = blockSize / 2;
    for(int j = halfSize; j<nl - halfSize -1; j++){
        uchar* data = binary.ptr<uchar>(j);
        int* idata1 = iimage.ptr<int>(j - halfSize);
        int* idata2 = iimage.ptr<int>(nc - halfSize  +1)

        for(int i = halfSize; j<nc - halfSize -1; i++){
            int sum = (idata2[i + halfSize + 1] - idata2[i - halfSize] - idata1[i + halfSize + 1] + idata1[i - halfSize]) / (blockSize*blockSize);

            if(data[i]< (sum - threshold))
                data[i] = 0;
            else 
                data[i] = 255;
        }
    }

    int64 time;
    time = cv::getTickCount();
    cv::adaptiveThreshold(image, binaryAdaptive, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, blockSize, threshold);


    time = cv::getTickCount()-time;
    cv::namedWindow("Adaptive Threshold (integral)");
    cv::imshow("Adaptive Threshold (integral)", binary);

    cv::Mat filtered;
    cv::Mat binaryFiltered;

    cv::boxFilter(image, filtered, CV_8U, cv::Size(blockSize, blockSize));
    binaryFiltered = image >= (filtered - threshold);
    time =cv::getTickCount - time;
    
    // IntegralImage<int, 1> integral(image);
    // cv::Mat test(image, cv::Rect(18,45,30,50));
    // cv::Scalar t = cv::sum(test);
    // cv::namedWindow("FixedThreshold");
    // cv::imshow("Fixed Threshold", binaryFixed);
cv::waitKey(0);
    return 0;

};