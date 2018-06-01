#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "../headers/houghFunctions.h"
using namespace std;
using namespace cv;

// This file calculates the hough space for an image passed in as a parameter.

/** @function main */
int main( int argc, const char** argv )
{
    // 1. Read Input Image
	Mat src = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    string s = argv[1] ;

    //string s = "coins1.png";

    //Mat src = imread(s) ;


    hough( src, s  );

	return 0;
}
