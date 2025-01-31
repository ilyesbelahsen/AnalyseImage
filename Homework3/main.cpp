#include "ContourDetector.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>

using namespace cv;
using namespace std;

int main() {
    string inputFolder = "./images";  
    string contoursFolder = "./contours"; 
    string outputFolder = "./resultats"; 

    int detectorChoice;
    cout << "Choisissez un détecteur :\n1. Canny\n2. Laplacian\n3. Sobel\nEntrez le numéro correspondant et appuyez sur Entrée: ";
    cin >> detectorChoice;

    ContourDetector detecteur; 

    for (const auto& entry : filesystem::directory_iterator(inputFolder)) {

        string img = entry.path().string();
        Mat original = imread(img, IMREAD_GRAYSCALE);

        if (original.empty()) {
            cout << "Erreur : impossible de charger l'image " << img << endl;
            continue;
        }

        string imgName = entry.path().stem().string();
        string imgResult = contoursFolder + "/" + imgName + ".jpg";
        Mat reference = imread(imgResult, IMREAD_GRAYSCALE);

        if (reference.empty()) {
            cout << "Erreur : impossible de charger l'image de référence " << imgResult << endl;
            continue;
        }

        detecteur.detectionMethod(detectorChoice, original, reference, imgName, outputFolder);

        cout << "Appuyez sur Entrée pour passer à l'image suivante..." << endl;
        cin.ignore();  
        cin.get();
    }

    return 0;
}
