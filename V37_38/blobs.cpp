#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::Mat image = cv::imread("binaryGroup.bmp", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Erreur : Impossible de charger l'image binaryGroup.bmp" << std::endl;
        return -1;
    }

    cv::namedWindow("Binary Image");
    cv::imshow("Binary Image", image);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    cv::Mat result(image.size(), CV_8U, cv::Scalar(255));
    cv::drawContours(result, contours, -1, cv::Scalar(0), 2);

    cv::namedWindow("Contours");
    cv::imshow("Contours", result);

    int cmin = 50;
    int cmax = 500;
    auto itc = contours.begin();
    while (itc != contours.end()) {
        if (itc->size() < cmin || itc->size() > cmax) {
            itc = contours.erase(itc);
        } else {
            ++itc;
        }
    }

    cv::Mat original = cv::imread("group.jpg", cv::IMREAD_GRAYSCALE);
    if (original.empty()) {
        std::cerr << "Erreur : Impossible de charger l'image group.jpg" << std::endl;
        return -1;
    }

    cv::drawContours(original, contours, -1, cv::Scalar(255), 2);

    cv::namedWindow("Contours on animals");
    cv::imshow("Contours on animals", original);

    result.setTo(cv::Scalar(255));
    cv::drawContours(result, contours, -1, 0, 1);

    cv::namedWindow("All contours");
    cv::imshow("All contours", result);

    image = cv::imread("binaryGroup.bmp", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Erreur : Impossible de charger l'image binaryGroup.bmp" << std::endl;
        return -1;
    }

    cv::Rect r0 = cv::boundingRect(contours[0]);
    cv::rectangle(result, r0, 0, 2);

    float radius;
    cv::Point2f center;
    cv::minEnclosingCircle(contours[1], center, radius);
    cv::circle(result, center, static_cast<int>(radius), 0, 2);

    std::vector<cv::Point> poly;
    cv::approxPolyDP(contours[2], poly, 5, true);
    cv::polylines(result, poly, true, 0, 2);

    std::vector<cv::Point> hull;
    cv::convexHull(contours[3], hull);
    cv::polylines(result, hull, true, 0, 2);

    itc = contours.begin();
    while (itc != contours.end()) {
        cv::Moments mom = cv::moments(*itc++);
        if (mom.m00 != 0) {
            cv::circle(result, cv::Point(mom.m10 / mom.m00, mom.m01 / mom.m00), 2, cv::Scalar(0), 2);
        }
    }

    cv::namedWindow("Some Shape Descriptors");
    cv::imshow("Some Shape Descriptors", result);

    cv::Mat components = cv::imread("mser.bmp", cv::IMREAD_GRAYSCALE);
    if (components.empty()) {
        std::cerr << "Erreur : Impossible de charger l'image mser.bmp" << std::endl;
        return -1;
    }

    components = components == 255;
    cv::morphologyEx(components, components, cv::MORPH_OPEN, cv::Mat(), cv::Point(-1, -1), 3);

    cv::namedWindow("MSER image");
    cv::imshow("MSER image", components);

    contours.clear();
    cv::Mat componentsInv = 255 - components;
    cv::findContours(componentsInv, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    cv::Mat quadri(components.size(), CV_8U, 255);
    for (auto& contour : contours) {
        poly.clear();
        cv::approxPolyDP(contour, poly, 5, true);

        if (poly.size() == 4) {
            cv::polylines(quadri, poly, true, 0, 2);
        }
    }

    cv::namedWindow("MSER quadrilateral");
    cv::imshow("MSER quadrilateral", quadri);

    cv::waitKey(0);
    return 0;
}
