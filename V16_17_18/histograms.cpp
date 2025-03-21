#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "histogram.h" 

int main()
{
    cv::Mat image= cv::imread("group.jpg",0); //  0 convertit en niveau gris
    if (!image.data)
        return 0;

    Histogram1D h;
    cv::Mat histo= h.getHistogram(image); // retourne un histogram indiquant les niveau de gris dans l'image et indique combien de pixel on une intensité donné

    for (int i=0; i<256; i++)
        std::cout << "Value " << i << " = " << histo.at<float>(i) << std::endl;

    cv::namedWindow("Histogram");
    cv::imshow("Histogram",h.getHistogramImage(image));

    cv::Mat hi = h.getHistogramImage(image);
    cv::line(hi, cv::Point(70, 0), cv::Point(70, 255), cv::Scalar(128)); // dessine une ligne vertical a la pos x=70

    cv::namedWindow("Histogram with threshold value");
    cv::imshow("Histogram with threshold value", hi);

    cv::Mat thresholded;
    cv::threshold(image, thresholded, 70, 255, cv::THRESH_BINARY);

    /*
cv::threshold :
Applique un seuillage binaire à l'image.
Tous les pixels avec une intensité supérieure ou égale à 70 sont définis à 255 (blanc).
Les autres pixels sont définis à 0 (noir).
    */

    cv::namedWindow("Binary Image");
    cv::imshow("Binary Image", thresholded);

    thresholded = 255 - thresholded;
    cv::imwrite("binary.bmp", thresholded);


   cv::Mat str = h.stretch(image, 0.01);
    cv::imshow("Stretched Image", str);

    cv::namedWindow("Stretched H");
    cv::imshow("Stretched H", h.getHistogramImage(str));

    // Créer une table de correspondance pour l'image négative
    cv::Mat lut(1, 256, CV_8U);
    for (int i = 0; i < 256; i++) {
        lut.at<uchar>(i) = 255 - i;
    }

    cv::namedWindow("Negative image");
    cv::imshow("Negative image", h.applyLookUp(image, lut));

    cv::Mat equalizedImage = h.equalize(image);

    cv::namedWindow("Equalized Image");
    cv::imshow("Equalized Image", equalizedImage);

    cv::namedWindow("Equalized H");
    cv::imshow("Equalized H", h.getHistogramImage(equalizedImage));

    cv::waitKey();
    return 0;
}
