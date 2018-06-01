/////////////////////////////////////////////////////////////////////////////
//
// COMS30121 - face.cpp
//
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
#include "../headers/groundtruth.h"  
#include "../headers/f1score.h"
#include "../headers/utility.h" 
using namespace std;
using namespace cv;

// Calculate the TPR for faces in either images 5 or 15. 
// This code was used to generate the sum for the TPR in the report (section 1). 

/** @function main */
int main( int argc, const char** argv )
{
       // 1. Read Input Image
	Mat image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    string s = argv[1] ;

    String cascade_name = "frontalface.xml";
    CascadeClassifier cascade;

    //load the classifier
    if( !cascade.load( cascade_name ) )
    {
        printf("--(!)Error loading\n");
        return -1;
    };

    std::vector<Rect> candidates  = detect(image, cascade);
    GroundTruths gt;
    if (strcmp(argv[1], "test/dart5.jpg") ==0)
    {
       std::cout << tpr(gt.get5(),  candidates) << endl;
       visualise(image, gt.get5(), candidates, s);
    } else if (strcmp(argv[1], "test/dart15.jpg") ==0)
    {
        std::cout<< tpr(gt.get15(), candidates) << endl;
        visualise(image, gt.get15(), candidates, s);

    } else {
        std::cout << "error: test/dart5.jpg or test/dart15.jpg only " << endl;
    }

	return 0;
}
