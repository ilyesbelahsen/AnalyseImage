#include <iostream>
#include <vector>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>

class laplacianZC
{
private:
    cv::Mat laplace;
    int aperture;

public:
    laplacianZC() : aperture(3) {}

    void setAperture(int a)
    {
        aperture = a;
    }

    int getAperture() const
    {
        return aperture;
    }

    cv::Mat computeLaplacian(const cv::Mat& image)
    {
        cv::Laplacian(image, laplace, CV_32F, aperture);
        return laplace;
    }

    cv::Mat getLaplacianImage(double scale = -1.0)
    {
        if(scale < 0)
        {
            double lapmin, lapmax;
            cv::minMaxLoc(laplace, &lapmin, &lapmax);
            scale = 127 / std::max(-lapmin, lapmax);
        }

        cv::Mat laplaceImage;
        laplace.convertTo(laplaceImage, CV_8U, scale, 128);
        return laplaceImage;
    }

    cv::Mat getZeroCrossings(cv::Mat laplace)
    {
        cv::Mat signImage;
        cv::threshold(laplace, signImage, 0, 255, cv::THRESH_BINARY);

        cv::Mat binary;
        signImage.convertTo(binary, CV_8U);

        cv::Mat dilated;
        cv::dilate(binary, dilated, cv::Mat());

        return dilated - binary;
    }
};
