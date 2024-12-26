#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


int main()
{
    cv::Mat image1, image2;

    image1 = cv::imread("boldt.jpg");
    image2 = cv::imread("rain.jpg");

    if (!image1.data) {
        return 0;
    }
    if (image2.empty()) {
        return 0;
    }

    cv::namedWindow("Image 1");
    cv::imshow("Image 1", image1);

    cv::namedWindow("Image 2");
    cv::imshow("Image 2", image2);

    cv::Mat result;
    cv::addWeighted(image1, 0.7, image2, 0.9, 0.0, result); //superposer les images
    
    
     cv::namedWindow("result");
     cv::imshow("result", result);
    //same
    // result=0.7*image1+0.9*image2;
    // cv::namedWindow("result with overloaded operators");
    // cv::imshow("result with overloaded opereatos", result);

// --- Modification du canal bleu de l'image 1
    std::vector<cv::Mat> planes; 

    cv::split(image1, planes);  

    cv::Mat grayImage2; 
    cv::cvtColor(image2, grayImage2, cv::COLOR_BGR2GRAY); // Convertir image2 en niveaux de gris
    planes[0] += grayImage2; // Ajouter image2 au canal bleu

    cv::merge(planes, result);

    cv::namedWindow("Result on blue channel");
    cv::imshow("Result on blue channel", result);

    cv::waitKey(0);

    return 0;
}
