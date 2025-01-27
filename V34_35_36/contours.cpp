#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "contours.hpp"

int main()
{
    cv::Mat image = cv::imread("road.jpg", cv::IMREAD_GRAYSCALE);
    if(image.empty()) 
    {
        std::cerr << "Error loading image!" << std::endl;
        return -1;
    }

    cv::namedWindow("Original Image");
    cv::imshow("Original Image", image);

    EdgeDetector ed;
    ed.computeSobel(image);

    cv::imwrite("ori.bmp", ed.getSobelOrientationImage());

    cv::namedWindow("Sobel (low threshold)");
    cv::imshow("Sobel (low threshold)", ed.getBinaryMap(125));

    cv::namedWindow("Sobel (high threshold)");
    cv::imshow("Sobel (high threshold)", ed.getBinaryMap(350));

    cv::Mat contours;
    cv::Canny(image, contours, 125, 350);

    cv::namedWindow("Canny Contours");
    cv::imshow("Canny Contours", 255 - contours);

    std::vector<cv::Vec2f> lines;
    cv::HoughLines(contours, lines, 1, CV_PI / 180, 50);
    
    cv::Mat result = image.clone();
    
    for (const auto& line : lines)
    {
        float rho = line[0];
        float theta = line[1];

        if (theta < CV_PI / 4. || theta > 3. * CV_PI / 4.)
        {
            cv::Point pt1(rho / cos(theta), 0);
            cv::Point pt2((rho - image.rows * sin(theta)) / cos(theta), result.rows);
            cv::line(result, pt1, pt2, cv::Scalar(255), 1);
        }
        else
        {
            cv::Point pt1(0, rho / sin(theta));
            cv::Point pt2(result.cols, (rho - result.cols * cos(theta)) / sin(theta));
            cv::line(result, pt1, pt2, cv::Scalar(255), 1);
        }
    }

    cv::namedWindow("Lines with Hough");
    cv::imshow("Lines with Hough", result);

    LineFinder ld;
    ld.setLineLengthAndGap(100, 20);
    ld.setMinVote(60);

    std::vector<cv::Vec4i> li = ld.findLines(contours);
    ld.drawDetectedLines(image);
    cv::namedWindow("Lines with HoughP");
    cv::imshow("Lines with HoughP", image);

    cv::Mat acc(200, 180, CV_8U, cv::Scalar(0));
    int x = 50, y = 30;

    for (int i = 0; i < 180; i++)
    {
        double theta = i * CV_PI / 180.;
        double rho = x * cos(theta) + y * sin(theta);
        int j = static_cast<int>(rho + 100.5);
        acc.at<uchar>(j, i)++;
    }

    cv::line(acc, cv::Point(0, 0), cv::Point(0, acc.rows - 1), 255);
    cv::line(acc, cv::Point(acc.cols - 1, acc.rows - 1), cv::Point(0, acc.rows - 1), 255);
    
    cv::imwrite("hough1.bmp", 255 - (acc * 100));

    x = 30; 
    y = 10;

    for (int i = 0; i < 180; i++)
    {
        double theta = i * CV_PI / 180.;
        double rho = x * cos(theta) + y * sin(theta);
        int j = static_cast<int>(rho + 100.5);
        acc.at<uchar>(j, i)++;
    }

    cv::namedWindow("Hough Accumulator");
    cv::imshow("Hough Accumulator", acc * 100);

    image = cv::imread("chariot.jpg", cv::IMREAD_GRAYSCALE);
    if(image.empty()) 
    {
        std::cerr << "Error loading image!" << std::endl;
        return -1;
    }

    cv::GaussianBlur(image, image, cv::Size(5, 5), 1.5);
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT, 2, 20, 200, 60, 15, 50);

    image = cv::imread("chariot.jpg", cv::IMREAD_GRAYSCALE);
    if(image.empty()) return -1;

    for (const auto& circle : circles)
    {
        cv::circle(image, cv::Point(circle[0], circle[1]), circle[2], cv::Scalar(255), 2);
    }

    cv::namedWindow("Detected Circles");
    cv::imshow("Detected Circles", image);

    image = cv::imread("road.jpg", cv::IMREAD_GRAYSCALE);
    if(image.empty()) return -1;

    int n = 0;
    cv::Mat oneline(image.size(), CV_8U, cv::Scalar(0));
    cv::line(oneline, cv::Point(li[n][0], li[n][1]), cv::Point(li[n][2], li[n][3]), cv::Scalar(255));
    cv::bitwise_and(contours, oneline, oneline);

    cv::namedWindow("One line");
    cv::imshow("One line", 255 - oneline);

    std::vector<cv::Point> points;

    for (int y = 0; y < oneline.rows; y++)
    {
        uchar* rowPtr = oneline.ptr<uchar>(y);
        for (int x = 0; x < oneline.cols; x++)
        {
            if (rowPtr[x])
                points.push_back(cv::Point(x, y));
        }
    }

    cv::Vec4f line;
    cv::fitLine(points, line, cv::DIST_L2, 0, 0.01, 0.01);

    int x0 = line[2];
    int y0 = line[3];
    int x1 = x0 + 100 * line[0];
    int y1 = y0 + 100 * line[1];
    
    image = cv::imread("road.jpg", cv::IMREAD_GRAYSCALE);
    if(image.empty()) return -1;
    
    cv::line(image, cv::Point(x0, y0), cv::Point(x1, y1), 0, 2);

    cv::namedWindow("Fitted line");
    cv::imshow("Fitted line", image);

    cv::waitKey(0);
    return 0;
}
