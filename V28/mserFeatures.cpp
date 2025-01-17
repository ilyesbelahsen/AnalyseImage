#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv4/opencv2/features2d.hpp>
#include <vector>


int main()
{
    cv::Mat image = cv::imread("building.jpg",0);
    if(image.empty()) 
        return 0;

    cv::namedWindow("Image");
    cv::imshow("Image",image);

    cv::Ptr<cv::MSER> ptrMSER = cv::MSER::create(5,200,2000);

    std::vector<std::vector<cv::Point>> points;
    std::vector<cv::Rect> rects;
    ptrMSER->detectRegions(image,points,rects);

    cv::Mat output(image.size(),CV_8UC3);
    output = cv::Scalar(255,255,255);

    cv::RNG rng;

    for(std::vector<std::vector<cv::Point>>::reverse_iterator it = points.rbegin(); it!=points.rend(); ++it)
    {
        cv::Vec3b c(rng.uniform(0,254),
                    rng.uniform(0,254),
                    rng.uniform(0,254));
        
        for(std::vector<cv::Point>::iterator itPts = it->begin(); itPts!=it->end(); ++itPts)
        {
            if(output.at<cv::Vec3b>(*itPts)[0] == 255)
            {
                output.at<cv::Vec3b>(*itPts) = c;
            }
        }
    }

    cv::namedWindow("MSER point sets");
    cv::imshow("MSER point sets",output);
    cv::imwrite("mser.bmp",output);

    std::vector<cv::Rect>:: iterator itr = rects.begin();
    std::vector<std::vector<cv::Point>>::iterator itp = points.begin();

    for(; itr != rects.end(); ++itr, ++itp)
    {
        if(static_cast<double>(itp->size())/itr->area() > 0.6)
            cv::rectangle(image, *itr, cv::Scalar(255),2);
    }

    cv::namedWindow("Rectangular MSERs");
    cv::imshow("Rectangular MSERs",image);

    image = cv::imread("building.jpg",0);
    if(image.empty()) return 0;

    for(std::vector<std::vector<cv::Point>>::iterator it = points.begin(); it != points.end(); ++it)
    {
        for(std::vector<cv::Point>:: iterator itPts = it->begin(); itPts != it->end(); ++itPts)
        {
            cv::RotatedRect rr = cv::minAreaRect(*it);

            if(rr.size.height / rr.size.height > 0.6 || rr.size.height / rr.size.height < 1.6)
                cv::ellipse(image, rr, cv::Scalar(255),2);
        }
    }

    cv::namedWindow("MSERs ellipses");
    cv::imshow("MSERs ellipses",image);

    cv::waitKey(0);
    return 0;
}
