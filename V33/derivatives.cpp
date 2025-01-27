#include "laplacianZC.hpp"

int main()
{
    cv::Mat image = cv::imread("boldt.jpg", 0);
    if (image.empty()) 
    {
        return 0;
    }

    cv::Mat sobelX, sobelY;
    cv::Sobel(image, sobelX, CV_8U, 1, 0, 3, 0.4, 128);
    cv::imshow("Sobel X Image", sobelX);

    cv::Sobel(image, sobelY, CV_8U, 0, 1, 3, 0.4, 128);
    cv::imshow("Sobel Y Image", sobelY);

    cv::Sobel(image, sobelX, CV_16S, 1, 0); 
    cv::Sobel(image, sobelY, CV_16S, 0, 1);

    cv::Mat sobel = abs(sobelX) + abs(sobelY);

    double sobmin, sobmax;
    cv::minMaxLoc(sobel, &sobmin, &sobmax);

    cv::Sobel(image, sobelX, CV_8U, 1, 0, 7, 0.001, 128);
    cv::imshow("Sobel X Image (7x7)", sobelX);

    cv::Mat sobelImage;
    sobel.convertTo(sobelImage, CV_8U, -255.0 / sobmax, 255);
    cv::imshow("Sobel Image", sobelImage);

    cv::Mat sobelThresholded;
    cv::threshold(sobelImage, sobelThresholded, 225, 255, cv::THRESH_BINARY);
    cv::imshow("Binary Sobel Image (low)", sobelThresholded);

    cv::threshold(sobelImage, sobelThresholded, 190, 255, cv::THRESH_BINARY);
    cv::imshow("Binary Sobel Image (high)", sobelThresholded);

    laplacianZC laplacian;
    laplacian.setAperture(7);

    cv::Mat flap = laplacian.computeLaplacian(image);
    double lapmin, lapmax;
    cv::minMaxLoc(flap, &lapmin, &lapmax);

    cv::Mat laplace = laplacian.getLaplacianImage();
    cv::imshow("Laplacian Image (7x7)", laplace);

    int cx = 238, cy = 90, dx = 12, dy = 12;
    std::cout << "Image values:\n";
    for (int i = 0; i < dx; i++)
    {
        for (int j = 0; j < dy; j++)
        {
            std::cout << std::setw(5) << static_cast<int>(image.at<uchar>(i + cy, j + cx)) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Laplacian value range = [" << lapmin << ", " << lapmax << "]\n";

    cv::Mat zeros = laplacian.getZeroCrossings(flap);
    cv::imshow("Zero-crossings", 255 - zeros);

    cv::Mat gauss20, gauss22, dog;
    cv::GaussianBlur(image, gauss20, cv::Size(), 2.0);
    cv::GaussianBlur(image, gauss22, cv::Size(), 2.2);

    cv::subtract(gauss22, gauss20, dog, cv::Mat(), CV_32F);
    zeros = laplacian.getZeroCrossings(dog);
    cv::imshow("Zero-crossings of DoG", 255 - zeros);

    cv::rectangle(image, cv::Rect(cx, cy, dx, dy), cv::Scalar(255, 255, 255));
    cv::imshow("Original Image with window", image);

    cv::waitKey(0);

    return 0;
}