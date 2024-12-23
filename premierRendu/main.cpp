#include <opencv2/opencv.hpp>
#include <iostream>

// ma fonction red line permettant de dessiner une ligne rouge
void redLine(cv::Mat& img, int y) {
    if (y >= 0 && y < img.rows) {
        for (int x = 0; x < img.cols; ++x) {
            img.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255);
        }
    }
}

// ma fonction pour tracer et afficher le graph d'intensité
void plotGraphIntensity(const cv::Mat& image, int y) {
    if (y < 0 || y >= image.rows) {
        std::cerr << "Ligne hors limites !" << std::endl;
        return;
    }

    int graphHeight = 300;
    cv::Mat graph(graphHeight, image.cols, CV_8UC3, cv::Scalar(255, 255, 255));

    for (int x = 1; x < image.cols; ++x) {
        int intensity1 = image.at<uchar>(y, x - 1);
        int intensity2 = image.at<uchar>(y, x);

        int y1 = graphHeight - (intensity1 * graphHeight / 255);
        int y2 = graphHeight - (intensity2 * graphHeight / 255);

        cv::line(graph, cv::Point(x - 1, y1), cv::Point(x, y2), cv::Scalar(0, 0, 0), 1);
    }

    cv::imshow("Profil d'intensité", graph);
}

bool lineSelected = false;
cv::Point selectedPoint;

void onMouseClick(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        selectedPoint = cv::Point(x, y);
        lineSelected = true;
        std::cout << "Coordonnées : x = " << x << ", y = " << y << std::endl;
    }
}

int main() {
    // ici je charge mon image
    cv::Mat image = cv::imread("ilyes.jpg", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Impossible de charger l'image" << std::endl;
        return -1;
    }
    // ici je convertie mon image en couleur afin de dessiner les lignes rouge
    cv::Mat colorImage;
    cv::Mat originalImage;
    cv::cvtColor(image, colorImage, cv::COLOR_GRAY2BGR);
    originalImage = colorImage.clone();

    cv::Mat displayImage = originalImage.clone();

    cv::namedWindow("Image Original");
    cv::setMouseCallback("Image Original", onMouseClick);

    while (true) {
        if (lineSelected) {
            // lors du clique je commence par :
            // reinitialiser l'image avant de dessiner la nouvelle ligne rouge
            displayImage = originalImage.clone(); 
            // dessiner la ligne rouge à l'endroit cliqué
            redLine(displayImage, selectedPoint.y);
            // afficher le graph d'intensité
            plotGraphIntensity(image, selectedPoint.y);
            // reinitialiser l'etat du clique
            lineSelected = false;
        }

        cv::imshow("Image Original", displayImage);

        if (cv::waitKey(30) >= 0) {
            break;
        }
    }

    return 0;
}
