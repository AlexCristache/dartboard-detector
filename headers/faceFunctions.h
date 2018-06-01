#ifndef FACEFUNCTIONS_H 
#define FACEFUNCTIONS_H 
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


//used to detect and display viola janes cascade classifier.
std::vector<Rect> detectAndDisplay(cv::Mat frame, cv::CascadeClassifier cascade); 


#endif 
