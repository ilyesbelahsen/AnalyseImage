#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// Fonction pour réduire les couleurs dans une image (méthode destructrice)
void colorReduce(cv::Mat image, int div = 64) {
    int nl = image.rows;  
    int nc = image.cols * image.channels();  // Nombre de colone multiplié par le nombre de canaux

    for (int j = 0; j < nl; j++) {
        uchar* data = image.ptr<uchar>(j);  // Pointeur sur la ligne j de l'image
        for (int i = 0; i < nc; i++) {
            // Réduction des couleurs pour le pixel `i`
            data[i] = data[i] / div * div + div / 2;
            data[i] = data[i]%div * div + div / 2;
        }
    }
}

// Fonction pour réduire les couleurs dans une image (entrée et sortie séparées)
void colorReduceIO(const cv::Mat& image, cv::Mat& result, int div = 64) {

    int nl = image.rows; 
    int nc = image.cols; 
    int nchannels = image.channels(); 

    result.create(image.rows, image.cols, image.type()); 


    for (int j = 0; j < nl; j++) {

        const uchar* data_in = image.ptr<uchar>(j);  // Pointeur sur la ligne j de l'image d'entrée
        uchar* data_out = result.ptr<uchar>(j);  // Pointeur sur la ligne j de l'image de sortie

        for (int i = 0; i < nc*nchannels; i++) {
            // Réduction des couleurs pour le pixel i
            data_out[i] = data_in[i] / div * div + div / 2;
        }
    }
}

void colorReduce6(cv::Mat image, int div = 64) {
    int nl = image.rows;
    int nc = image.cols * image.channels();

    // Si les pixels de l'image est stocké de maniere continue en mémoire sans interruption de ligne, tout est traité comme une seule ligne (optimisation de traitement)
    if (image.isContinuous()) {
        nc = nc * nl;
        nl = 1;  // Une seule ligne
    }

    // Calcul pour le masque et l'ajustement des couleurs
    int n = static_cast<int>(log(static_cast<double>(div)) / log(2.0) + 0.5); // Base-2 log pour obtenir le nombre de bits significatifs

    uchar mask = 0xFF << n;  // Masque pour réduire les couleurs
    uchar div2 = div >> 1;   // div divisé par 2 pour ajuster la moyenne

    for (int j = 0; j < nl; j++) {
        uchar* data = image.ptr<uchar>(j);  

        // Parcourir les pixels de la ligne
        for (int i = 0; i < nc; i++) {
            *data = (*data & mask) + div2;  // Réduction des couleurs
            *data++ += div2;  // Passer au pixel suivant
        }
    }
}

void colorReduce7(cv::Mat image, int div = 64) {

    if(image.isContinuous()){
        image.reshape(1,1);
    }

    int nl = image.rows;
    int nc = image.cols * image.channels();

    int n = static_cast<int>(log(static_cast<double>(div)) / log(2.0) + 0.5); 

    uchar mask = 0xFF << n;  
    uchar div2 = div >> 1;  

    for (int j = 0; j < nl; j++) {
        uchar* data = image.ptr<uchar>(j);  

        for (int i = 0; i < nc; i++) {
            *data = (*data & mask) + div2;  
            *data++ += div2;  
        }
    }
}

void colorReduce10(cv::Mat image, int div = 64) {
    int n = static_cast<int>(log(static_cast<double>(div)) / log(2.0) + 0.5); 
    uchar mask = 0xFF << n;  // mask prend le bit 0xFF (11111111) et si n = 2 alors mask =  11111100 = 252
    uchar div2 = div >> 1;  

    cv::Mat_<cv::Vec3b>::iterator it = image.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itend = image.end<cv::Vec3b>();

    // Parcourir chaque pixel de l'image
    for (; it != itend; ++it) {
        (*it)[0] &= mask;  // Appliquer le masque au canal bleu  : fait un & bit a bit 
        (*it)[0] += div2;  // Ajuster avec div/2

        (*it)[1] &= mask;  //  canal vert
        (*it)[1] += div2;  

        (*it)[2] &= mask;  // canal rouge
        (*it)[2] += div2;  

        /*        
        Avec un pixel ayant les valeurs [100, 150, 200] :
        Masque pour div = 64, n = 6 : mask = 0b11000000 (192).
        Canal bleu [100] : 100 & 192 = 64 (réduction).
                           64 + 32 = 96 (ajustement).
        Canal vert [150] : 150 & 192 = 128.
                           128 + 32 = 160.
        Canal rouge [200] : 200 & 192 = 192.
                            192 + 32 = 224.
        Résultat final : [96, 160, 224].
        */

    }
}

void colorReduce11(cv::Mat image, int div = 64) {
    cv::Mat_<cv::Vec3b> cimage = image; //copie

    cv::Mat_<cv::Vec3b>::iterator it = cimage.begin();
    cv::Mat_<cv::Vec3b>::iterator itend = cimage.end();

    uchar div2 = div >> 1; 

    for (; it != itend; ++it) {
        (*it)[0] = (*it)[0] / div * div + div2; 
        (*it)[1] = (*it)[1] / div * div + div2; 
        (*it)[2] = (*it)[2] / div * div + div2;
    }
}

void  colorReduce5(cv::Mat image, int div = 64) {
    int nl = image.rows; 
    int n = static_cast<int>(log(static_cast<double>(div)) / log(2.0) + 0.5); 
    uchar mask = 0xFF << n; 

    for (int j = 0; j < nl; j++) {
        uchar* data = image.ptr<uchar>(j); 

        for (int i = 0; i < image.cols * image.channels(); i++) {
            *data &= mask;  
            *data++ += div/2; 
        }
    }
}

void colorReduce12(cv::Mat image, int div = 64) {
    int nl = image.rows;  
    int nc = image.cols;  
    uchar div2 = div >> 1; 

    for (int j = 0; j < nl; j++) {
        for (int i = 0; i < nc; i++) {
            image.at<cv::Vec3b>(j, i)[0] = image.at<cv::Vec3b>(j, i)[0] / div * div + div2; 
            image.at<cv::Vec3b>(j, i)[1] = image.at<cv::Vec3b>(j, i)[1] / div * div + div2;  
            image.at<cv::Vec3b>(j, i)[2] = image.at<cv::Vec3b>(j, i)[2] / div * div + div2; 
        }
    }
}

int main() {
    cv::Mat image = cv::imread("boldt.jpg");

    int div = 64; 

    cv::Mat imageClone;

    imageClone = image.clone();
    colorReduce(imageClone, div);
    cv::imshow("colorReduce", imageClone);

    imageClone = image.clone();
    colorReduce5(imageClone, div);
    cv::imshow("colorReduce5", imageClone);

    imageClone = image.clone();
    colorReduce6(imageClone, div);
    cv::imshow("colorReduce6", imageClone);

    imageClone = image.clone();
    colorReduce7(imageClone, div);
    cv::imshow("colorReduce7", imageClone);

    imageClone = image.clone();
    colorReduce10(imageClone, div);
    cv::imshow("colorReduce10", imageClone);

    imageClone = image.clone();
    colorReduce11(imageClone, div);
    cv::imshow("colorReduce11", imageClone);

    imageClone = image.clone();
    colorReduce12(imageClone, div);
    cv::imshow("colorReduce12", imageClone);

    cv::Mat result;
    imageClone = image.clone();
    colorReduceIO(imageClone, result, div);
    cv::imshow("colorReduceIO", result);

    cv::waitKey(0);

    return 0;
}





