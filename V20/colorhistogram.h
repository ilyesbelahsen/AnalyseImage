#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

class ColorHistogram {
private:
    int histSize[3];       // Nombre de bins pour chaque canal
    float hranges[2];      // Gamme de valeurs pour les bins (min et max)
    const float* ranges[3]; // Tableau de gammes pour chaque canal
    int channels[3];       // Canaux à analyser

public:
    ColorHistogram() {
        // Initialisation des paramètres par défaut
        histSize[0] = histSize[1] = histSize[2] = 256; // 256 bins
        hranges[0] = 0.0; // Min
        hranges[1] = 256.0; // Max
        ranges[0] = hranges; // Toutes les gammes identiques
        ranges[1] = hranges;
        ranges[2] = hranges;
        channels[0] = 0; // Par défaut, le canal 0
        channels[1] = 1;
        channels[2] = 2;
    }

    cv::Mat getHueHistogram(const cv::Mat& image, int minSaturation = 0) {
        cv::Mat hist;

        // Conversion de l'image en HSV
        cv::Mat hsv;
        cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

        cv::Mat mask;
        if (minSaturation > 0) {
            // Création d'un masque basé sur la saturation minimale
            std::vector<cv::Mat> v;
            cv::split(hsv, v); // Séparation des canaux (H, S, V)
            cv::threshold(v[1], mask, minSaturation, 255, cv::THRESH_BINARY);
        }

        // Définition des paramètres de l'histogramme
        hranges[0] = 0.0;   // Hue min
        hranges[1] = 180.0; // Hue max (pour HSV)
        channels[0] = 0;    // Canal Hue

        // Calcul de l'histogramme avec masque (si fourni)
        cv::calcHist(&hsv,
                     1,         // 1 seule image
                     channels,  // Canal à analyser
                     mask,      // Masque
                     hist,      // Histogramme à remplir
                     1,         // Histogramme 1D
                     histSize,  // Taille de l'histogramme
                     ranges     // Gamme des valeurs
        );

        return hist;
    }
};
