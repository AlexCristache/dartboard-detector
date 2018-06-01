/////////////////////////////////////////////////////////////////////////////
//
// COMS30121 - face.cpp
// Used to run viola jones using the classifier provided as global variable.
/////////////////////////////////////////////////////////////////////////////

// header inclusion
// header inclusion
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "../headers/faceFunctions.h"

using namespace std;
using namespace cv;


/** Global variables */
String cascade_name = "cascade.xml";
//String cascade_name = "frontalface.xml";
CascadeClassifier cascade;


/** @function main */
int main( int argc, const char** argv )
{
       // 1. Read Input Image
	Mat frame = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    string s = argv[1] ;
	// 2. Load the Strong Classifier in a structure called `Cascade'
	if( !cascade.load( cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	// 3. Detect Faces and Display Result
	detectAndDisplay( frame, cascade );

	// 4. Save Result Image
	imwrite( s + "detected.jpg", frame );

	return 0;
}

