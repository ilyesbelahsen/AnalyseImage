#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

using namespace cv;
using namespace std;


class ContourDetector {
public:
    int threshold1Canny = 100, threshold2Canny = 200;
    int laplacianSize = 3;  
    int sobelSize = 3;    

    
    void resultsFolder(const Mat& generated, const string& outputFolder, const string& imgName, const string& detectorName) {
        string outputPath = outputFolder + "/" + imgName + "_" + detectorName + ".png"; 
        if (!filesystem::exists(outputFolder)) {
            filesystem::create_directory(outputFolder); 
        }

         if (!imwrite(outputPath, generated)) {
        cout << "Erreur : Impossible d'enregistrer l'image à : " << outputPath << endl;
    } else {
        cout << "Image enregistrée à : " << outputPath << endl;
    }
    }

    void writeResults(const string& outputFolder, const string& imgName, const string& detectorName, double contoursDetectes, double contoursReference, double contoursCorrects, double fauxPositifs, double fauxNegatifs, double performance, double tauxFauxPositifs, double tauxFauxNegatifs) {
               
        ofstream resultFile(outputFolder + "/comparison_results.txt", ios::app);

        if (resultFile.is_open()) {
            resultFile << "Image : " << imgName << " méthode utilisé : " << detectorName << endl;
            resultFile << "contours détectés : " << contoursDetectes << endl;
            resultFile << "contours référence : " << contoursReference << endl;
            resultFile << "contours corrects : " << contoursCorrects << endl;
            resultFile << "faux positifs : " << fauxPositifs << endl;
            resultFile << "faux négatifs : " << fauxNegatifs << endl;
            resultFile << "performance : " << performance << " %" << endl;
            resultFile << "taux faux positifs : " << tauxFauxPositifs << " %" << endl;
            resultFile << "taux faux negatifs : " << tauxFauxNegatifs << " %" << endl;
            resultFile << "---------------------------------------" << endl;
            resultFile.close();
        } 
    }

    void compareImage(const Mat& generated, const Mat& reference, const string& imgName, const string& outputFolder, const string& detectorName) {
        double contoursDetectes = countNonZero(generated);
        double contoursReference = countNonZero(reference);
        double contoursCorrects = countNonZero(generated & reference);
        double fauxPositifs = contoursDetectes - contoursCorrects;
        double fauxNegatifs = contoursReference - contoursCorrects;

        double performance = (contoursCorrects / (contoursCorrects + fauxPositifs + fauxNegatifs) ) * 100;
        double tauxFauxPositifs = (fauxPositifs / (contoursCorrects + fauxPositifs + fauxNegatifs) ) * 100;
        double tauxFauxNegatifs = (fauxNegatifs / (contoursCorrects + fauxPositifs + fauxNegatifs) ) * 100;

        writeResults(outputFolder, imgName, detectorName, contoursDetectes, contoursReference, contoursCorrects, fauxPositifs, fauxNegatifs, performance, tauxFauxPositifs, tauxFauxNegatifs);
    }

    void detectionMethod(int detectorsChoice, const Mat& imgOriginal, const Mat& imgReference, const string& imgName, const string& outputFolder) {

        namedWindow("Image Original", WINDOW_NORMAL);
        namedWindow("Image Générée", WINDOW_NORMAL);
        namedWindow("Contours Main", WINDOW_NORMAL);
        int gaussianSize = 5; 

        Mat generated, blurred;

        if (detectorsChoice == 1) {  
            createTrackbar("Threshold1", "Image Générée", &threshold1Canny, 255);
            createTrackbar("Threshold2", "Image Générée", &threshold2Canny, 255);

            while (true) {
                GaussianBlur(imgOriginal, blurred, Size(5, 5), 1.5);  

                Canny(blurred, generated, threshold1Canny, threshold2Canny);

                imshow("Image Original", imgOriginal);
                imshow("Image Générée", generated);
                imshow("Contours Main", imgReference);

                char key = (char)waitKey(30);
                if (key == 13) {  
                    resultsFolder(generated, outputFolder, imgName, "Canny");
                    compareImage(generated, imgReference,  imgName, outputFolder, "Canny");
                    break;
                }
            }
        } else if (detectorsChoice == 2) {  

            auto laplacianCallback = [](int pos, void* userData) {
                int& ksize = *(int*)userData;
                ksize = (pos % 2 == 1) ? pos : pos + 1;
            };

            createTrackbar("Taille du noyau (kernel)", "Image Générée", &laplacianSize, 31, laplacianCallback, &laplacianSize);

            while (true) {
                int kernelSizeUser = max(1, gaussianSize | 1);
                GaussianBlur(imgOriginal, blurred, Size(kernelSizeUser, kernelSizeUser), 0);
                
                //filtre laplacian
                Laplacian(blurred, generated, CV_16S, laplacianSize);
      
                double minVal, maxVal;
                minMaxLoc(generated, &minVal, &maxVal);
                generated.convertTo(generated, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));

                imshow("Image Original", imgOriginal);
                imshow("Image Générée", generated);
                imshow("Contours Main", imgReference);

                char key = (char)waitKey(30);
                if (key == 13) {  
                    resultsFolder(generated, outputFolder, imgName, "Laplacian");
                    compareImage(generated, imgReference,  imgName, outputFolder, "Laplacian");
                    break;
                }
            }
        } else if (detectorsChoice == 3) {  
            auto sobelCallback = [](int pos, void* userData) {
                int& ksize = *(int*)userData;
                ksize = (pos % 2 == 1) ? pos : pos + 1; 
            };

            createTrackbar("Taille du noyau (kernel)", "Image Générée", &sobelSize, 31, sobelCallback, &sobelSize);

            while (true) {
                int ksize = max(1, sobelSize | 1);
                Mat sobelX, sobelY, abs_sobelX, abs_sobelY;
                int kernelSizeUser = max(1, gaussianSize | 1);
                GaussianBlur(imgOriginal, blurred, Size(kernelSizeUser, kernelSizeUser), 0);

                Sobel(blurred, sobelX, CV_16S, 1, 0, ksize);
                Sobel(blurred, sobelY, CV_16S, 0, 1, ksize);
                Mat sobel = abs(sobelX) * 0.5 + abs(sobelY) * 0.5;

                double minVal, maxVal;
                minMaxLoc(sobel, &minVal, &maxVal);
                sobel.convertTo(generated, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));

                imshow("Image Original", imgOriginal);
                imshow("Image Générée", generated);
                imshow("Contours Main", imgReference);

                char key = (char)waitKey(30);
                if (key == 13) {  
                    resultsFolder(generated, outputFolder, imgName, "Sobel");
                    compareImage(generated, imgReference,  imgName, outputFolder, "Sobel");
                    break;
                }
            }
        }

        destroyAllWindows();
    }
};
