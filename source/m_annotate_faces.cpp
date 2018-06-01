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
#include "../headers/faceFunctions.h" 
using namespace std;
using namespace cv;

void annotate(cv::Mat image, std::vector<Rect> gTruth, std::vector<Rect> candidates, string imageName)
{
    for (int i = 0; i < gTruth.size(); i++)
    {
       	rectangle(image, Point(gTruth[i].x, gTruth[i].y), Point(gTruth[i].x
                    +gTruth[i].width, gTruth[i].y + gTruth[i].height), Scalar(
                        255, 255, 0 ), 2);
	}

    for (int i = 0; i < candidates.size(); i++)
    {
            rectangle(image, Point(candidates[i].x,
                        candidates[i].y),Point(candidates[i].x +candidates[i].width, candidates[i].y+ candidates[i].height), Scalar( 0, 255, 0 ), 2);
    }

    // 4. Save Result Image
    imwrite( imageName + "detected.jpg", image );
}

/** @function main */
int main( int argc, const char** argv )
{
       // 1. Read Input Image
	//Mat image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

    String cascade_name = "frontalface.xml";
    CascadeClassifier cascade;
    //load the classifier
    if( !cascade.load( cascade_name ) )
    {
        printf("--(!)Error loading\n");
        return -1;
    };

    GroundTruths gt;

    string name = "test/dart4.jpg";
    Mat image = imread(name, CV_LOAD_IMAGE_COLOR);
    std::vector<Rect> candidates  = detectAndDisplay(image, cascade);
    annotate(image, gt.get4(), candidates, name);

    name = "test/dart5.jpg";
    image = imread(name, CV_LOAD_IMAGE_COLOR);
    candidates  = detectAndDisplay(image, cascade);
    annotate(image, gt.get5(), candidates, name);

    name = "test/dart13.jpg";
    image = imread(name, CV_LOAD_IMAGE_COLOR);
    candidates  = detectAndDisplay(image, cascade);
    annotate(image, gt.get13(), candidates, name);

    name = "test/dart14.jpg";
    image = imread(name, CV_LOAD_IMAGE_COLOR);
    candidates  = detectAndDisplay(image, cascade);
    annotate(image, gt.get14(), candidates, name);

    name = "test/dart15.jpg";
    image = imread(name, CV_LOAD_IMAGE_COLOR);
    candidates  = detectAndDisplay(image, cascade);
    annotate(image, gt.get15(), candidates, name);

	return 0;
}
