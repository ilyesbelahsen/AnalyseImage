#include <opencv2/opencv.hpp>
#include <iostream>

cv::Mat EtirerImage(const cv::Mat &image){
    //commencer par trouver les valeur d'intensiter max et min afin des les etirer a 0 - 255
    double val_min, val_max;

    cv::minMaxLoc(image, &val_min, &val_max);
    std::cout << "la plage d'intensiter intial est [" << val_min << ", " << val_max << "]" << std::endl;

    //etendre maintenant la plage
    cv::Mat imageEtirer;
    image.convertTo(imageEtirer, CV_8U, 255.0 / (val_max - val_min), - val_min * (255.0 / (val_max - val_min)));

    return imageEtirer;
}

cv::Mat getHistogram(const cv::Mat &image){
    int histSize = 256;
    float range[] = {0, 256};
    const float *histRanges = {range};
    int channels[] = {0};

    cv::Mat hist;
    cv::calcHist(
        &image,                 // Image source
        1,                      // Une seule image
        channels,                // Canal à analyser
        cv::Mat(),              // Pas de masque
        hist,                   // Histogramme de sortie
        1,                      // Histogramme 1D
        &histSize,              // Nombre de bins
        &histRanges             // Plage des valeurs
    );

    return hist;
}

static cv::Mat getHistogramImage(const cv::Mat &hist, int histH, int histW) {
    double maxVal = 0;
    double minVal = 0;
    cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);
                
    cv::Mat histImg(histH, histW, CV_8U, cv::Scalar(255));

    int barre = cvRound((double)histW / 256);

    for (int i = 1; i < 256; i++) {
        cv::line(histImg,
            cv::Point(barre * (i - 1), histH - cvRound(hist.at<float>(i - 1) * histH / maxVal)),
            cv::Point(barre * i, histH - cvRound(hist.at<float>(i) * histH / maxVal)),
            cv::Scalar(0), 
            2); 
    };

    return histImg;
}

int main() {
    cv::Mat image = cv::imread("AI_totoro.png", cv::IMREAD_GRAYSCALE);
    if (!image.data) {
        return 0;
    }

    cv::Mat imageEtirer = EtirerImage(image);

    cv::Mat histOriginal = getHistogram(image);
    cv::Mat histEtirer = getHistogram(imageEtirer);

    cv::Mat histImgOriginal = getHistogramImage(histOriginal, 400, 512);
    cv::Mat histImgEtirer = getHistogramImage(histEtirer, 400, 512);

    cv::imshow("Image Originale", image);
    cv::imshow("Histogramme Original", histImgOriginal);
    cv::imshow("Image Étendue", imageEtirer);
    cv::imshow("Histogramme Étendu", histImgEtirer);

    cv::waitKey(0);
    return 0;
}
