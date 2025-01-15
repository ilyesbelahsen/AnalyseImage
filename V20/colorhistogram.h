#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

class ColorHistogram {
private:
    int histSize[3];       
    float hranges[2];      
    const float* ranges[3]; 
    int channels[3];      

public:
    ColorHistogram() {
        
        histSize[0] = histSize[1] = histSize[2] = 256; 
        hranges[0] = 0.0;
        hranges[1] = 256.0; 
        ranges[0] = hranges; 
        ranges[1] = hranges;
        ranges[2] = hranges;
        channels[0] = 0; 
        channels[1] = 1;
        channels[2] = 2;
    }

    cv::Mat getHueHistogram(const cv::Mat& image, int minSaturation = 0) {
        cv::Mat hist;

        cv::Mat hsv;
        cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

        cv::Mat mask;
        if (minSaturation > 0) {
            std::vector<cv::Mat> v;
            cv::split(hsv, v); 
            cv::threshold(v[1], mask, minSaturation, 255, cv::THRESH_BINARY);
        }

        hranges[0] = 0.0;   
        hranges[1] = 180.0;
        channels[0] = 0;    

        cv::calcHist(&hsv,
                     1,         
                     channels, 
                     mask,      
                     hist,     
                     1,       
                     histSize,  
                     ranges   
        );

        return hist;
    }
};
