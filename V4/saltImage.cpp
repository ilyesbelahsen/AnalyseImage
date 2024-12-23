#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <random>

void salt(cv::Mat image, int n) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> randomRow(0, image.rows - 1);
    std::uniform_int_distribution<int> randomCol(0, image.cols - 1);

    int i, j;
    for (int k = 0; k < n; k++) {
        i = randomCol(generator);
        j = randomRow(generator);
        
        //mettre le pixel blanc
        if (image.type() == CV_8UC1) {
            image.at<uchar>(j, i) = 255; // uchar pour acceder a un pixel dans une image niveau de gris (1 canal)
        } else if (image.type() == CV_8UC3) {
            image.at<cv::Vec3b>(j, i)[0] = 255; //cv::Vec3b pour acceder a un pixel dans une image couleur
            image.at<cv::Vec3b>(j, i)[1] = 255; // les indice correspond aux canaux de couleur (blue, green, red)
            image.at<cv::Vec3b>(j, i)[2] = 255;
        }
    }
}

int main() {
    cv::Mat image = cv::imread("boldt.jpg", 1);
    salt(image, 3000);
    cv::namedWindow("Image");
    cv::imshow("Image", image);

    cv::waitKey(0);
    return 0;
}