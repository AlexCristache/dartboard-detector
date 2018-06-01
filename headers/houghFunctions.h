#ifndef HOUGHFUNCTIONS_H
#define HOUGHFUNCTIONS_H

#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "utility.h"


using namespace std;
using namespace cv;

//threshold the pixels in an image by setting those higher than
//@value to 255 and the others to 0
void threshold2(Mat image, int value);

//calculates the euclidean distance between 2 points
float distance(int x1, int y1,int  x2, int y2) ;

//populate the 3D array with the votes for each pixel to be the center of a circle
std::vector<Mat> generate_accumulator(Mat grad, int min_radius, int max_radius, Mat theta, int step);

//generate the Hough space from the 3D array of votes
Mat  houghSpace(std::vector<Mat> accumulator);

//iterate through the 3D array of votes and only keep the circles with a score bigger than @circle_threshold
std::vector<std::vector<int>>thresholdCircles(std::vector<Mat> accumulator, int circle_threshold, int min_radius, int step );

//check if a point is close enough to the center of another already found circle
int contains(std::vector<std::vector<int>> circles, int x, int y);

//filter the thresholded circles to display only one circle for a cluster of centers
std::vector<std::vector<int>>getConcentricCircles(std::vector<std::vector<int>> relevantCircles);

//function that draws circles on a given image
void drawHoughCircles(Mat image, std::vector<std::vector<int>> relevantCircles,std::vector<Mat> accumulator);

//calculate the minimum threshold for a circle to be considered viable by
//only considering the upper @percentage of values in the 3D array
int getThreshold(std::vector<Mat> accumulator, float percentage) ;

//perform the hough transform
std::vector<std::vector<int>> hough(Mat src, string imageName);
#endif
