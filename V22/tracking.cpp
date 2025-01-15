int main(){
    cv:Mat image = cv::imread('bike55.bmp');

    int xo =97, yo=112;
    int width=25, height=30;
    cv::Mat roi(image, cv::Rect(xo, yo, width, height));

    cv:Scalar sum = cv::sum(roi);

    cv::Mat integralImage;  
    cv::integral(image, integralImage, CV_32S);

    int sumInt = integralImage.at<int>(yo+height, xo+width) - integralImage.at<int>(yo+height, xo) - integralImage.at<int>(yo, wo+width) + integralImage.at<int>(yo, xo);

    std::cout << sumInt << std::endl;

    Histogram1D h;
    h.setNbins(16);
    cv::Mat refHistogram = h.getHistogram(roi);
}