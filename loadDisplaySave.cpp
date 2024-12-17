#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

void onMouse(int event, int x, int y, int flags, void* param){
  cv::Mat *im = reinterpret_cast<cv::Mat*>(param);

  switch (event)
  {
  case cv::EVENT_LBUTTONDOWN:
    std::cout << "at (" << x << "," << y << ") value is: "
    << static_cast<int>(im->at<uchar>(cv::Point(x,y))) << std::endl;
    break;
  
  default:
    break;
  }
}

cv::Mat function(){
  cv::Mat ima(500,500,CV_8U,50);
  return ima;
}



int main() {
  
  cv::Mat image;
  std::cout << "this image is" << image.rows << "x" << image.cols << std::endl;

  image = cv::imread("raouf.jpeg", cv::IMREAD_COLOR);

  if(image.empty()){

  }

  cv::namedWindow("Original Image");
  cv::imshow("Original Image", image);

  cv::setMouseCallback("Original Image", onMouse, reinterpret_cast<void*>(&image));

  // cv::Mat result;
  // cv::flip(image, result, 1);

  // cv::namedWindow("Output Image");
  // cv::imshow("Output Image", result);
  // cv::imwrite("output.bmp", result);

  cv::namedWindow("Drawing on an Image");

  cv::circle(image, 
    cv::Point(100,110),
    65,
    0,
    3);

  cv::putText(image, "j'suis une merde", cv::Point(10,150), cv::FONT_HERSHEY_PLAIN, 1.0, 255, 2);

  cv::imshow("Drawing on an Image", image);

  cv::waitKey(0);

    return 0;
}