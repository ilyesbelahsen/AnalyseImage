#ifndef WATERSHED_SEGMENTER_H
#define WATERSHED_SEGMENTER_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

class WatershedSegmenter {
private:
    cv::Mat markers;

public:
    void setMarkers(const cv::Mat& markerImage) {
        markerImage.convertTo(markers, CV_32S);
    }

    cv::Mat process(const cv::Mat& image) {
        cv::watershed(image, markers);
        return markers;
    }

    cv::Mat getSegmentation() const {
        cv::Mat tmp;
        markers.convertTo(tmp, CV_8U);
        return tmp;
    }

    cv::Mat getWatersheds() const {
        cv::Mat tmp;
        markers.convertTo(tmp, CV_8U, 255, 255);
        return tmp;
    }
};

#endif 
