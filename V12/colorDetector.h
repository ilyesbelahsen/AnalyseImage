#ifndef COLORDETECTOR_H
#define COLORDETECTOR_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <cstdlib> 

class ColorDetector {
private:
    cv::Vec3b target;  
    cv::Mat converted; 
    int maxDist;            
    bool useLab;  
    
    cv::Mat result;          
    

public: 
   
    ColorDetector() : target(0, 0, 0), converted(), maxDist(100), useLab(false), result() {};
    ColorDetector(bool useLab) : target(0, 0, 0), converted(), maxDist(100), useLab(useLab), result() {};
    ColorDetector(uchar blue, uchar green, uchar red, int mxDist = 100, bool useLab = false)
        : target(0, 0, 0), converted(), maxDist(mxDist), useLab(useLab), result() {
        setTargetColor(blue, green, red);
    }

    int getDistanceToTargetColor(const cv::Vec3b &color) const {
        return getColorDistance(color, target);
    }

    int getColorDistance(const cv::Vec3b &color1, const cv::Vec3b &color2) const {
        return std::abs(color1[0] - color2[0]) +
               std::abs(color1[1] - color2[1]) +
               std::abs(color1[2] - color2[2]);
/* 
        //or
        return static_cast<int>(
            cv::norm<int,3>(cv::Vec3i(color[0]-color2[1], color[1]-color2[1], color[2]-color2[2]))
        );

        //or
        cv::Vec3b dist;
        cv::absdiff(color, color2, dist);
        return cv::sum(dist)[0];
*/
    }

    cv::Mat process(const cv::Mat &image);

    cv::Mat operator()(const cv::Mat &image){
        cv::Mat input;
        if(useLab){
            cv::cvtColor(image, input, cv::COLOR_BGR2Lab);
        }
        else{
            input = image;
        }

        cv::Mat output;
        cv::absdiff(input,cv::Scalar(target), output);

        std::vector<cv::Mat> images;
        cv::split(output, images);

        output= images[0]+images[1]+images[2];
        cv::threshold(output, output, maxDist, 255, cv::THRESH_TOZERO_INV);
        
        return output;
    }

    void setColorDistanceThreshold(int distance) { 
        if (distance < 0) { 
            distance = 0; 
        }
        maxDist = distance; 
    }

    int getColorDistanceThreshold() const{
        return maxDist; 
    }

    void setTargetColor(uchar blue, uchar green, uchar red) {
        target = cv::Vec3b(blue, green, red); 

        if(useLab){
            cv::Mat tmp(1,1, CV_8UC3);
            tmp.at<cv::Vec3b>(0,0) = cv::Vec3b(blue, green, red);
            cv::cvtColor(tmp,tmp,cv::COLOR_BGR2Lab);
            target = tmp.at<cv::Vec3b>(0,0);
        }
    }

    void setTargetColor(cv::Vec3b color){
        target = color;
    }

    cv::Vec3b getTargetColor() const {
        return target;
    }
    
};

#endif
