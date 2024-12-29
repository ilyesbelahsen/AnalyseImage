#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>

//fonction qui iddentifie les pixels d'une img qui ont une teinte et une saturation dans une plage donné
void detectHScolor(const cv::Mat& image,
    double minHue, double maxHue,
    double minSat, double maxSat,
    cv::Mat& mask) {    // mask : img niveau gris ou les pixels correspondant au critere sont blancs et le reste noir

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV); //conversion hsv

    std::vector<cv::Mat> channels;
    cv::split(hsv, channels); //division en canaux hsv

    //mask pour la teinte  
    cv::Mat mask1;   
    cv::threshold(channels[0], mask1, maxHue, 255, cv::THRESH_BINARY_INV); //mask1 garde les pixels avec une teinte < maxhue
    cv::Mat mask2; 
    cv::threshold(channels[0], mask2, minHue, 255, cv::THRESH_BINARY); //mask1 garde les pixels avec une teinte > minhue

    cv::Mat hueMask;
    if (minHue < maxHue)
        hueMask = mask1 & mask2;
    else
        hueMask = mask1 | mask2;
    //idem mask pour la saturation
    cv::threshold(channels[1], mask1, maxSat, 255, cv::THRESH_BINARY_INV);
    cv::threshold(channels[1], mask2, minSat, 255, cv::THRESH_BINARY);

    cv::Mat satMask;
    satMask = mask1 & mask2;

//combine les deux mask
    mask = hueMask & satMask;
}

int main() {
    cv::Mat image = cv::imread("boldt.jpg");
    if (!image.data)
        return 0;

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV); //convertion de l'espace BGR vers HSV Hue(teinte) Saturation Value(luminosité)

    std::vector<cv::Mat> channels;
    cv::split(hsv, channels); // divise l'image en 3 canaux H S V

/*
Hue (teinte) : Décrit la couleur en degrés (0 à 360, mais ici représenté de 0 à 180 pour OpenCV).
Saturation (saturation) : Décrit l'intensité de la couleur (0 = gris, 255 = couleur vive).
Value (luminosité) : Décrit la luminosité (0 = sombre, 255 = brillant).
*/

    cv::namedWindow("avant divison en 3 canaux");
    cv::imshow("avant divison en 3 canaux", hsv);

    cv::namedWindow("canal 3 : Luminosité");
    cv::imshow("canal 3 : Luminosité", channels[2]);

    cv::namedWindow("canal 2 : Saturation");
    cv::imshow("canal 2 : Saturation", channels[1]);

    cv::namedWindow("canal 1 : Teinte");
    cv::imshow("canal 1 : Teinte", channels[0]);

    //modifie la luminosité en la mettant max (255) sur tout les pixel
    cv::Mat newImage;
    cv::Mat tmp(channels[2].clone());
    channels[2] = 255;
    cv::merge(channels, hsv);
    cv::cvtColor(hsv, newImage, cv::COLOR_HSV2BGR);
    cv::namedWindow("luminosité max");
    cv::imshow("luminosité max", newImage);

    //modifie la saturation en la mettant max (255) sur tout les pixel
    channels[1] = 255;
    channels[2] = tmp;
    cv::merge(channels, hsv);
    cv::cvtColor(hsv, newImage, cv::COLOR_HSV2BGR);
    cv::namedWindow("saturation max");
    cv::imshow("saturation max", newImage);

    //image où l'axe horizontal (de 0 à 360) représente la teinte et l'axe vertical (de 0 à 128) représente la saturation
    cv::Mat hs(128, 360, CV_8UC3);
    for (int h = 0; h < 360; h++) {
        for (int s = 0; s < 128; s++) {
            hs.at<cv::Vec3b>(s, h)[0] = h / 2;
            hs.at<cv::Vec3b>(s, h)[1] = 255 - s * 2;
            hs.at<cv::Vec3b>(s, h)[2] = 255;
        }
    }
    cv::cvtColor(hs, newImage, cv::COLOR_HSV2BGR);
    cv::namedWindow("f = Teinte(Saturation)");
    cv::imshow("f = Teinte(Saturation)", newImage);

    image = cv::imread("girl.jpg");
    if (!image.data)
        return 0;

    cv::namedWindow("Original image");
    cv::imshow("Original image", image);

    cv::Mat mask;
    detectHScolor(image, 160, 10, 25, 166, mask);

    cv::Mat detected(image.size(), CV_8UC3, cv::Scalar(0, 0, 0)); //creer une image de la meme taille que l'origine et l'initialise en noir
    image.copyTo(detected, mask);
    cv::imshow("Application de detectHScolor", detected);

    cv::Mat linear(100, 256, CV_8U);
    for (int i = 0; i < 256; i++) {
        linear.col(i) = i;
    }
    cv::namedWindow("Linear Gradient");
    cv::imshow("Linear Gradient", linear);

    cv::waitKey(0);
    return 0;
}
