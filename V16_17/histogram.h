#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>


class Histogram1D {
    private:
        int histSize[1];
        float hranges[2];
        const float* ranges[1];
        int channels[1];

    public:
        Histogram1D(){
            histSize[0]=histSize[1]=histSize[2]=256;
            hranges[0]=0.0;
            hranges[1] = 256.0;
            ranges[0] =hranges;
            ranges[1] =hranges;
            ranges[2] =hranges;
            channels[0]= 0;
            channels[1]= 1;
            channels[2]= 2;
        }

        void setChannel(int c){
            channels[0] = c;
        }

        float getMaxValue(){
            return hranges[1];
        }
        float getNBins(){
            return histSize[0];
        }

        void setNBins(int nbins){
            histSize[0] = nbins;
        }

        cv::Mat getHistogram(const cv::Mat &image){
            cv::Mat hist;

            cv::calcHist(&image, 1, channels, cv::Mat(), hist, 1, histSize, ranges);

            return hist;
        }

        cv::Mat getHistogramImage(const cv::Mat &image, int zoom = 1) {
            cv::Mat hist = getHistogram(image);
            return Histogram1D::getImageOfHistogram(hist, zoom);
        }

        cv::Mat stretch(const cv::Mat &image, float percentile) {
            float number = image.total()*percentile;
            cv::Mat hist = getHistogram(image);

            int imin = 0;
            for (float count =0.0; imin < 256; imin++) {
                if ((count+=hist.at<float>(imin)) >= number)
                    break;
            }

            int imax = 255;
            for (float count=0.0; imax >= 0; imax--) {
                if ((count += hist.at<float>(imax)) > number)
                    break;
            }

            int dims[1] = {256};
            cv::Mat lookup(1, dims, CV_8U);

            for (int i = 0; i < 256; i++) {
                if (i < imin) lookup.at<uchar>(i) = 0;
                else if (i > imax) lookup.at<uchar>(i) = 255;
                else lookup.at<uchar>(i) = cvRound(255.0 * (i - imin) / (imax - imin));
            }

            cv::Mat result;
            result = applyLookUp(image, lookup);

            

            return result;
        }

        static cv::Mat getImageOfHistogram(const cv::Mat &hist, int zoom) {
            double maxVal = 0;
            double minVal = 0;
            cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);

            int histSize = hist.rows;
            cv::Mat histImg(histSize * zoom, histSize * zoom, CV_8U, cv::Scalar(255));

            int hpt = static_cast<int>(0.9 * histSize);

            for (int h = 0; h < histSize; h++) {
                float binVal = hist.at<float>(h);
                if (binVal > 0) {
                    int intensity = static_cast<int>(binVal * hpt / maxVal);
                    cv::line(histImg, cv::Point(h * zoom, histSize * zoom),
                            cv::Point(h * zoom, (histSize - intensity) * zoom),
                            cv::Scalar(0), zoom);
                }
            }
            return histImg;
        }

        static cv::Mat applyLookUp(const cv::Mat& image, const cv::Mat& lookup) {
            cv::Mat result;
            cv::LUT(image, lookup, result);
            return result;
        }

        static cv::Mat applyLookUpWithIterator(const cv::Mat& image, const cv::Mat& lookup) {
            cv::Mat result(image.rows, image.cols, CV_8U);
            cv::Mat_<uchar>::iterator itr = result.begin<uchar>();

            cv::Mat_<uchar>::const_iterator it = image.begin<uchar>();
            cv::Mat_<uchar>::const_iterator itend = image.end<uchar>();

            for (; it != itend; ++it, ++itr) {
                *itr = lookup.at<uchar>(*it);
            }

            return result;
        }

};