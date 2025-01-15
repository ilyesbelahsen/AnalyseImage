#include <opencv2/opencv.hpp>  
#include <iostream>
#include "../V16_17_18/histogram.h" 
#include "integral.h" 
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


int main(){
    cv::Mat image = cv::imread("bike55.bmp");  
    int xo = 97, yo = 112;
    int width = 25, height = 30;

    cv::Mat roi(image, cv::Rect(xo, yo, width, height));

    // cv::Scalar sum = cv::sum(roi);  

    cv::Mat integralImage;
    cv::integral(image, integralImage, CV_32SC1);

    int sumInt = integralImage.at<int>(yo + height, xo + width) 
               - integralImage.at<int>(yo + height, xo) 
               - integralImage.at<int>(yo, xo + width)  
               + integralImage.at<int>(yo, xo);

    std::cout << sumInt << std::endl;


    cv::Mat secondImage = cv::imread("bike65.bmp", 0);
    if (!secondImage.data)
        return 0;


    Histogram1D h;
    h.setNBins(16);
    cv::Mat refHistogram = h.getHistogram(roi);

    

    IntegralImage<float, 16> intHistogram(secondImage);

    double maxSimilarity = 0.0;
    double distance = 0.0;
    int xbest = 0, ybest = 0;

    for (int y = 110; y < 120; y++) {
        for (int x = 0; x < secondImage.cols - width; x++) {
            cv::Vec<float, 16> histogram = intHistogram(x, y, width, height);  
            distance = cv::compareHist(refHistogram, histogram, cv::HISTCMP_INTERSECT);
            if (distance > maxSimilarity) {
                xbest = x;
                ybest = y;
                maxSimilarity = distance;
            }
        }
    }

    cv::rectangle(image, cv::Rect(xo, yo, width, height), 0);

    cv::rectangle(secondImage, cv::Rect(xbest, ybest, width, height), 0);  
    cv::rectangle(secondImage, cv::Rect(0, 110, secondImage.cols, height + 10), 255);   

    cv::imshow("Original Image", image); 
    cv::imshow("Second Image", secondImage); 

    cv::waitKey(0);

    return 0;
}
