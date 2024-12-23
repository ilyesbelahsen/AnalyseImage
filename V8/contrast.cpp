#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//accentué une image

//methode manuelle
void sharpen(const cv::Mat &image, cv::Mat &result) {

    result.create(image.size(), image.type());
    int nchannels = image.channels(); 

    for (int j = 1; j < image.rows - 1; j++) {
        const uchar* previous = image.ptr<const uchar>(j - 1);
        const uchar* current = image.ptr<const uchar>(j);
        const uchar* next = image.ptr<const uchar>(j + 1);
        uchar *output = result.ptr<uchar>(j);

        for (int i = nchannels; i < (image.cols - 1) * nchannels; i++) {
            *output++ = cv::saturate_cast<uchar>(
                5 * current[i] - current[i - nchannels] - current[i + nchannels] - previous[i] - next[i]
            );
            // on multiplie par 5 le pixel et on soustrait ces voisins, saturate_cast assure que le resultat reste entre 0 et 255
        }
    }

    //gere les bord, ils sont remplie avec la valeur 0 (noir)
    result.row(0).setTo(cv::Scalar(0));        // Première ligne
    result.row(result.rows - 1).setTo(cv::Scalar(0)); // Dernière ligne
    result.col(0).setTo(cv::Scalar(0));        // Première colonne
    result.col(result.cols - 1).setTo(cv::Scalar(0)); // Dernière colonne
}

// methode par convolution
// filtre de convolution
void sharpen2D(const cv::Mat &image, cv::Mat &result) {
    // Création d'un noyau de convolution 3x3 initialisé à 0
    cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));  

    kernel.at<float>(1, 1) = 5.0;  // Pixel central à 5
    kernel.at<float>(0, 1) = -1.0; // Pixel au-dessus
    kernel.at<float>(2, 1) = -1.0; // Pixel en dessous
    kernel.at<float>(1, 0) = -1.0; // Pixel à gauche
    kernel.at<float>(1, 2) = -1.0; // Pixel à droite
/*
[  0  -1   0 ]
[ -1   5  -1 ]
[  0  -1   0 ]
*/

    //applique la convolution avec le kerner sur l'image
    cv::filter2D(image, result, image.depth(), kernel);
}

int main() {
    cv::Mat image = cv::imread("boldt.jpg");
    if (!image.data) {
        return 0;
    }

    cv::Mat result;
    sharpen(image, result);

    cv::Mat result2;
    sharpen2D(image, result2);

    cv::imshow("Original Image", image);
    cv::imshow("Sharpened Image", result);
    cv::imshow("Sharpened Image 2D", result2);

    cv::waitKey(0);

    return 0;
}

