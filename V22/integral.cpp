#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "integral.h"

int main() {
    cv::Mat image = cv::imread("book.jpg", 0);  
    if (!image.data)
        return 0;

    cv::transpose(image, image);
    cv::flip(image, image, 0);
    cv::namedWindow("Original Image");
    cv::imshow("Original Image", image);

    cv::Mat binaryFixed;
    cv::threshold(image, binaryFixed, 70, 255, cv::THRESH_BINARY);

    cv::Mat iimage;
    cv::integral(image, iimage, CV_32S);

    int blockSize = 21;
    int threshold = 10;
    int halfSize = blockSize / 2;
    int nl = image.rows;
    int nc = image.cols;

    for (int j = halfSize; j < nl - halfSize - 1; j++) {
        uchar* data = binaryFixed.ptr<uchar>(j);  
        int* idata1 = iimage.ptr<int>(j - halfSize);
        int* idata2 = iimage.ptr<int>(j + halfSize + 1);  

        for (int i = halfSize; i < nc - halfSize - 1; i++) {  
            int sum = (idata2[i + halfSize + 1] - idata2[i - halfSize] 
                     - idata1[i + halfSize + 1] + idata1[i - halfSize]) / (blockSize * blockSize);

            if (data[i] < (sum - threshold))
            {
                data[i] = 0; 
            }
            else
            {
                data[i] = 255; 
            }
        }
    }
   
    cv::Mat binaryAdaptive;

    int64 time = cv::getTickCount();  
    cv::adaptiveThreshold(image, binaryAdaptive, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, blockSize, threshold);
    time = cv::getTickCount() - time; 

    cv::namedWindow("Adaptive Threshold (integral)");
    cv::imshow("Adaptive Threshold (integral)", binaryFixed); 

    cv::namedWindow("Adaptive Threshold (classic)");
    cv::imshow("Adaptive Threshold (classic)", binaryAdaptive);  

    cv::Mat filtered, binaryFiltered;
    cv::boxFilter(image, filtered, CV_8U, cv::Size(blockSize, blockSize));
    binaryFiltered = image >= (filtered - threshold);

    cv::namedWindow("Filtered Threshold");
    cv::imshow("Filtered Threshold", binaryFiltered); 

    cv::waitKey(0);
    return 0;
}
