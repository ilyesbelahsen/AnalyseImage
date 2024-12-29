#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath> 

//separe le premier plan d'une image
int main()
{

    cv::Mat image= cv::imread("boldt.jpg");
    if (!image.data)
        return 0;

    cv::Rect rectangle(50,25,210,180);
    //je defini le triangle (zone pour definir quoi extraire)

    cv::Mat result;
    cv::Mat bgModel,fgModel; // deux matrices

    cv::grabCut(image, result, rectangle, bgModel, fgModel, 5, cv::GC_INIT_WITH_RECT); // segmente l'image, result: matrice indiquant chaque pixel si fg ou bg
    cv::compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ); // filtre result et garde   fg


    cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(255, 255, 255)); // creation d'une img blanche 
    image.copyTo(foreground, result); // copie de result dans img blanche

    cv::rectangle(image, rectangle, cv::Scalar(255,255,255),1);

    cv::imshow("Image originale avec rectangle", image);
    cv::imshow("Premier plan extrait", foreground);

    cv::waitKey(0);
}

