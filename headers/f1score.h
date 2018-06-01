#ifndef F1SCORE_H
#define F1SCORE_H

#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "groundtruth.h"
#include "utility.h" 

using namespace std;
using namespace cv;

//compares two rectangles to see if satisfy overlap conditions. 
bool rectMatch(Rect gTruth, Rect candidate); 

// calculates the number of true positives using ground turth and candidates. 
float truePositives(std::vector<Rect> gTruths, std::vector<Rect> candidates); 

// Calculates the true positive rate using truePositives function. 
float tpr(std::vector<Rect> gTruths, std::vector<Rect> candidates); 

void f1Image(std::vector<Rect> candidates, string imageName) ; 
// Calculates the f1 score
float calculateF1(float precision, float recall); 

//Detects candidates in an image using a cascade classifier
std::vector<Rect> detect(Mat image, CascadeClassifier cascade); 
#endif 
