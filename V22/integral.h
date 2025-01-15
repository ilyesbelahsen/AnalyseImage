#ifndef INTEGRAL_H
#define INTEGRAL_H

// video 22 8:34

#include <opencv2/core.hpp>

template<typename T, int N>
class IntegralImage {
private:
    cv::Mat_<cv::Vec<T, N>> integralImage; 

public:
    IntegralImage(const cv::Mat& image) {
        integralImage = cv::Mat_<cv::Vec<T, N>>(image.size(), cv::Vec<T, N>(0)); 
        cv::integral(image, integralImage, cv::DataType<T>::type); 
    }

    cv::Vec<T, N> operator()(int xo, int yo, int width, int height) const {
        return integralImage(yo + height, xo + width)
             - integralImage(yo + height, xo)
             - integralImage(yo, xo + width)
             + integralImage(yo, xo);
    }

    cv::Vec<T, N> operator()(int x, int y, int radius) const {
        return integralImage(y + radius + 1, x + radius + 1)
             - integralImage(y + radius + 1, x - radius)
             - integralImage(y - radius + 1, x - radius)
             + integralImage(y - radius, x - radius);
    }

    void converToBinaryPlanes(const cv::Mat& input, cv::Mat& output, int nPlanes){
        int n = 8- static_cast<int>(log(static_cast<double>(nPlanes))/log(2.0));
        uchar mask = 0xFF<<n;

        std::vector<cv::Mat> planes;
        cv::Mat reduced = input&mask;
        
        for(int i = 0; i<nPlanes; i++){
            planes.push_back((reduced==(i<<n))&0x1);

        }

        cv::merge(planes, output);
    }
};

#endif 
