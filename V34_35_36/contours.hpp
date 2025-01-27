#ifndef CONTOURS_HPP
#define CONTOURS_HPP

#include <iostream>
#include <vector>

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>

class EdgeDetector
{
    private:
        cv::Mat sobelX, sobelY, sobel, sobelImage, sobelThresholded, norm, dir;
    public:
        EdgeDetector() {}

        void computeSobel(cv::Mat image)
        {
            cv::Sobel(image,     // input
                      sobelX,    // output
                      CV_8U,     // image type
                      1, 0,      // kernel specification
                      3,         // size of the square kernel
                      0.4, 128); // scale and offset

            cv::Sobel(image,
                      sobelY,
                      CV_8U,
                      0, 1,
                      3,
                      0.4, 128);

            cv::Sobel(image,sobelX,CV_16S,1,0);
            cv::Sobel(image,sobelY,CV_16S,0,1);
            sobel = abs(sobelX) + abs(sobelY);

            cv::Sobel(image,sobelX,CV_32F,1,0);
            cv::Sobel(image,sobelY,CV_32F,0,1);
            cv::cartToPolar(sobelX,sobelY,norm,dir);

            double sobmin, sobmax;
            cv::minMaxLoc(sobel,&sobmin,&sobmax);

            sobel.convertTo(sobelImage,CV_8U,-255./sobmax,255);
        }

        cv::Mat getSobelOrientationImage() { return dir; }

        cv::Mat getBinaryMap(int thres)
        {
            if(thres > 255)
                thres = 255;
            cv::threshold(sobelImage, sobelThresholded, thres, 255, cv::THRESH_BINARY);
            return sobelThresholded;
        }
};

class LineFinder
{
    private:
        cv::Mat img;
        std::vector<cv::Vec4i> lines;

        double deltaRho;
        double deltaTheta;
        int minVote;
        double minLength;
        double maxGap;

    public:
        LineFinder() : deltaRho(1), deltaTheta(M_PI/180), minVote(10), minLength(0.), maxGap(0.) {}

        void setAccResolution(double dRho, double dTheta)
        {
            deltaRho = dRho;
            deltaTheta = dTheta;
        }

        void setMinVote(int minv)
        {
            minVote = minv;
        }

        void setLineLengthAndGap(double length, double gap)
        {
            minLength = length;
            maxGap = gap;
        }

        std::vector<cv::Vec4i> findLines(cv::Mat& binary)
        {
            lines.clear();
            cv::HoughLinesP(binary,lines,
                            deltaRho,deltaTheta,minVote,
                            minLength, maxGap);

            return lines;
        }

        void drawDetectedLines(cv::Mat &image,cv::Scalar color = cv::Scalar(255,255,255))
        {
            std::vector<cv::Vec4i>::const_iterator it2 = lines.begin();

            while(it2 != lines.end())
            {
                cv::Point pt1((*it2)[0],(*it2)[1]);
                cv::Point pt2((*it2)[2],(*it2)[3]);
                cv::line(image, pt1, pt2, color);
                ++it2;
            }
        }
};

#endif