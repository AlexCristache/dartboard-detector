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



// This code when executed will calculate the f1score across all images for a 
// viola jones classifier named "cascade.xml" in the directory from the program
// is being run. 

/** @function main */
int main( int argc, const char** argv )
{
    String cascade_name = "cascade.xml";
    CascadeClassifier cascade;
    //load the classifier
    if( !cascade.load( cascade_name ) )
    {
        printf("--(!)Error loading\n");
        return -1;
    };

    float tp_total = 0;
    float tpfn_total = 0;
    float tpfp_total = 0;
    int numberOfImages = atoi(argv[1]);
    GroundTruths gt;
    for(int i = 2; i < numberOfImages + 2; i++)
    {
        // 1. Read Input Image
        Mat image = imread(argv[i], CV_LOAD_IMAGE_COLOR);
        string s = argv[i];
        std::vector<Rect> candidates  = detect(image, cascade);
        std::vector<Rect> groundTruths = gt.getGT(s);
        float tpfn_image = groundTruths.size();
        float tpfp_image = candidates.size();
        float tp_image = truePositives(groundTruths, candidates);
        float precision_image = tp_image/tpfp_image;
        float recall_image = tp_image/tpfn_image;
        cout << "image " << i << endl;
        cout << "precision: " << precision_image << endl;
        cout << "recall: " << recall_image << endl;
        cout << "f1 score: " << calculateF1(precision_image, recall_image) << endl;
        cout << "-----------------------" << endl;
        tpfn_total += groundTruths.size();
        tpfp_total += candidates.size();
        tp_total += truePositives(groundTruths, candidates);
        visualise(image, groundTruths, candidates, s);
    }

    float precision = tp_total/tpfp_total;
    float recall = tp_total/tpfn_total;
    cout << "overall precision: " << precision << endl;
    cout << "overall recall: " << recall << endl;
    cout << "overall f1 score: " << calculateF1(precision, recall) << endl;
 	return 0;
}
