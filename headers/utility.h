#ifndef UTILITY_H 
#define UTILITY_H 


#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

// displays an image in a window. 
void displayImage(cv::Mat image, string name); 

//draws ground truth and candidates on an image 
void visualise(Mat image, std::vector<Rect> gTruth, std::vector<Rect> candidates, string imageName); 

#endif 


 
