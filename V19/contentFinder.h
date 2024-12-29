#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>

class ContentFinder {
private:
    float hranges[2];
    const float* ranges[3];
    int channels[3];
    float threshold;
    cv::Mat histogram;

    cv::SparseMat shistogram;
    bool isSparse;

public:
    ContentFinder() : threshold(0.1f), isSparse(false) {
        ranges[0] = hranges;
        ranges[1] = hranges;
        ranges[2] = hranges;
    }

    void setThreshold(float t){
        threshold = t;
    }

    float getThreshold(){
        return threshold;
    }
   

void setHistogram(const cv::Mat& h) {
            isSparse = false;
            cv::normalize(h, histogram, 1.0);
        }

        void setHistogram(const cv::SparseMat& h) {
            isSparse = true;
            cv::normalize(h, shistogram, 1.0, cv::NORM_L2);
        }

        cv::Mat find(const cv::Mat& image) {
            cv::Mat result;

            hranges[0] = 0.0;
            hranges[1] = 256.0;
            channels[0] = 0;
            channels[1] = 1;
            channels[2] = 2;

            return find(image, hranges[0], hranges[1], channels);
        }

        cv::Mat find(const cv::Mat& image, float minValue, float maxValue, int* channels) {
            cv::Mat result;

            hranges[0] = minValue;
            hranges[1] = maxValue;

            if (isSparse) {
                for (int i = 0; i < shistogram.dims(); i++) {
                    this->channels[i] = channels[i];
                }

                cv::calcBackProject(&image,
                                    1,
                                    channels,
                                    shistogram,
                                    result,
                                    ranges,
                                    255.0);
            } else {
                for (int i = 0; i < histogram.dims; i++) {
                    this->channels[i] = channels[i];
                }

                cv::calcBackProject(&image,
                                    1,
                                    channels,
                                    histogram,
                                    result,
                                    ranges,
                                    255.0);
            }

            if (threshold > 0.0) {
                cv::threshold(result, result, 255.0 * threshold, 255.0, cv::THRESH_BINARY);
            }

            return result;
        }


};