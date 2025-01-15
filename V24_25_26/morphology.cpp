#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat image = cv::imread("binary.bmp");
    if (!image.data) 
        return 0;

    cv::namedWindow("Original Image");
    cv::imshow("Original Image", image);

    cv::Mat element(7, 7, CV_8U, cv::Scalar(1));  
    cv::Mat eroded;
    cv::erode(image, eroded, element);
    cv::namedWindow("Eroded Image (7x7)");
    cv::imshow("Eroded Image (7x7)", eroded);

    cv::erode(image, eroded, cv::Mat(), cv::Point(-1, -1), 3);
    cv::namedWindow("Eroded Image (3 times)");
    cv::imshow("Eroded Image (3 times)", eroded);

    cv::Mat element5(5, 5, CV_8U, cv::Scalar(1)); 
    cv::Mat closed;
    cv::morphologyEx(image, closed, cv::MORPH_CLOSE, element5);
    cv::namedWindow("Closed Image");
    cv::imshow("Closed Image", closed);

    cv::Mat opened;
    cv::morphologyEx(image, opened, cv::MORPH_OPEN, element5);
    cv::namedWindow("Opened Image");
    cv::imshow("Opened Image", opened);

    cv::morphologyEx(image, image, cv::MORPH_CLOSE, element5);  
    cv::morphologyEx(image, image, cv::MORPH_OPEN, element5);  
    cv::namedWindow("Opened/Closed Image");
    cv::imshow("Opened/Closed Image", image);

    image = cv::imread("boldt.jpg",0);
    if(!image.data)
        return 0;

    cv::Mat result;

    cv::morphologyEx(image, result, cv::MORPH_GRADIENT, cv::Mat());

    cv::namedWindow("Edge Image");
    cv::imshow("Edge Image", 255 - result);

    image = cv::imread("book.jpg", 0);
    if(!image.data)
        return 0;
    
    cv::transpose(image, image);    
    cv::flip(image, image,0);

    cv::Mat element7(7,7, CV_8U, cv::Scalar(1));
    cv::morphologyEx(image, result, cv::MORPH_BLACKHAT, element7);

    cv::namedWindow("7x7 Black Top-hat Image");
    cv::imshow("7x7 Black Top-hat Image", 255 - result);

    cv::waitKey(0);
    return 0;
}
