#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv4/opencv2/features2d.hpp>

int main(){

    cv::Mat image = cv::imread("boldt.jpg",0);
    if(image.empty()) 
        return 0;

    cv::namedWindow("Original Image");
    cv::imshow("Original Image",image);

    cv::Mat result;
    cv::blur(image,result,cv::Size(5,5));

    cv::namedWindow("Mean filtered Image");
    cv::imshow("Mean filtered Image",result);

    cv::GaussianBlur(image,result,cv::Size(5,5),1.5);

    cv::namedWindow("Gaussian filtered Image");
    cv::imshow("Gaussian filtered Image",result);

    cv::Mat gauss = cv::getGaussianKernel(9,1.5,CV_32F);

    cv::GaussianBlur(image,image,cv::Size(11,11), 1.75);

    cv::Mat reduced(image.rows/4, image.cols/4, CV_8U);
    cv::Mat reduced2(image.rows/4, image.cols/4, CV_8U);
    for(int i = 0; i < reduced.rows; i++)
    {
        for(int j = 0; j < reduced.cols; j++)
        {
            reduced.at<uchar>(i,j) = image.at<uchar>(i*4,j*4);
            reduced2.at<uchar>(i,j) = image.at<uchar>(i*4,j*4);
        }
    }

    cv::imwrite("reducedimage.bmp",reduced);

    cv::namedWindow("Badly Reduced Image");
    cv::imshow("Badly Reduced Image",reduced);

    cv::Mat newImage;
    cv::resize(reduced, newImage, cv::Size(), 4, 4, cv::INTER_NEAREST);

    cv::namedWindow("Reduced Image");
    cv::imshow("Reduced Image",newImage);

    cv::resize(reduced, newImage, cv::Size(), 4, 4, cv::INTER_LINEAR);

    cv::namedWindow("Bilinear resizing");
    cv::imshow("Bilinear resizing",newImage);

    cv::Mat pyramid(image.rows, image.cols + image.cols/2 + image.cols/4 + image.cols/8, CV_8U,cv::Scalar(255));
    image.copyTo(pyramid(cv::Rect(0,0,image.cols,image.rows)));

    cv::pyrDown(image,reduced); 
    reduced.copyTo(pyramid(cv::Rect(image.cols,image.rows/2,image.cols/2,image.rows/2)));
    cv::pyrDown(reduced,reduced2); 
    reduced2.copyTo(pyramid(cv::Rect(image.cols+image.cols/2,image.rows-image.rows/4,image.cols/4,image.rows/4)));
    cv::pyrDown(reduced2,reduced); 
    reduced.copyTo(pyramid(cv::Rect(image.cols+image.cols/2+image.cols/4,image.rows-image.rows/8,image.cols/8,image.rows/8)));

    cv::namedWindow("Pyramid of images");
    cv::imshow("Pyramid of images",pyramid);

    cv::resize(reduced2,newImage,cv::Size(),4,4,cv::INTER_LINEAR);

    cv::namedWindow("Bilinear resizing");
    cv::imshow("Bilinear resizing",newImage);

    cv::medianBlur(image,result,7);

    cv::namedWindow("Median filtered Image");
    cv::imshow("Median filtered Image",result);

    cv::blur(result,result,cv::Size(5,5));

    cv::namedWindow("Median filtered S&P Image");
    cv::imshow("Median filtered S&P Image",result);

    reduced = cv::Mat(image.rows/4, image.cols/4, CV_8U);
    for(int i = 0; i < reduced.rows; i++)
    {
        for(int j = 0; j < reduced.cols; j++)
        {
            reduced.at<uchar>(i,j) = image.at<uchar>(i*4,j*4);
        }
    }
    cv::resize(reduced, newImage, cv::Size(), 4, 4, cv::INTER_NEAREST);

    cv::waitKey(0);
    return 0;
};