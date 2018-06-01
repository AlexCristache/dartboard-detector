#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include "../headers/faceFunctions.h"
#include "../headers/houghFunctions.h"
#include <stdio.h>
#include <math.h>
#include "../headers/f1score.h"
using namespace std;
using namespace cv;


// Takes a vector of hough circles and returns a vector of Rectangles
// Which is the corresponding bounding box.
std::vector<Rect> boundingBoxes(std::vector<std::vector<int>> circles)
{
    std::vector<Rect> houghRects;
    for(int i = 0; i < circles.size(); i++)
    {
        int x = circles[i][0];
        int y = circles[i][1];
        int r = circles[i][2];
        Point p1 = Point(y - r, x - r);
        Point p2 = Point(y + r, x + r);
        Rect candidate = Rect(p1, p2);
        houghRects.push_back(candidate);
    }
   return houghRects;
}


// Computes the shared region of two rectangles and to see if it's area is
// as big as a certain percentage of the smallest of the two original
// rectangles.
bool compareRegions(cv::Rect violaRect , cv::Rect houghRect, float percent)
{
    float area = (violaRect & houghRect).area();
    if (violaRect.area() > houghRect.area())
    {
        if( area > houghRect.area() * percent)
        {
            return true;
        }
    }
    else {
        if( area > violaRect.area() * percent)
        {
            return true;
        }
    }
    return false;
}

// Merges two rectangles into one new region and returns it.
// It finds the extremes of each x,y but then alters the difference
// by adding the distance/2 in respect to x, then y.
Rect mergeRegions(Rect r1, Rect r2)
{
    int xMin, yMin, xMax, yMax;
    if (r1.x < r2.x)
    {
        xMin = r1.x;
    } else
    {
        xMin = r2.x;
    }

    if (r1.y < r2.y)
    {
        yMin = r1.y;
    } else
    {
        yMin = r2.y;
    }

    if (r1.x+r1.width> r2.x+r2.width)
    {
        xMax = r1.x+r1.width;
    } else
    {
        xMax = r2.x + r2.width;
    }
    if (r1.y + r1.height > r2.y + r2.height)
    {
        yMax = r1.y + r1.height;
    } else {
        yMax = r2.y + r2.height;
    }

    int divider = 2;
    int distX = abs(r1.x - r2.x) / divider;
    int distY = abs(r1.y - r2.y) / divider;

    return Rect(Point(xMin+distX, yMin+distY), Point(xMax-distX, yMax+distY)) ;
}


// Resuasable Filtering code which takes two regions, and loops through primary
// first deciding only keeping each one, if it intersects with another region.
std::vector<Rect> filterOverlapping(std::vector<Rect> primary, std::vector<Rect> secondary, float intersect)
{
    std::vector<Rect> filtered;
    for (int i = 0; i < primary.size(); i++)
    {
        int counter = 0;
        Rect overlapRegion = primary[i];
        for (int j = 0; j < secondary.size(); j++)
        {
            if (compareRegions(overlapRegion, secondary[j], intersect))
            {
                //merge and increment regions if overlap occurs
                counter++;
                mergeRegions(overlapRegion, secondary[j]);
            }
        }
        // checks to see if any overlapping regions were found.
        if(counter > 0)
        {
            filtered.push_back(overlapRegion);
        }
    }
    return filtered;
}

// This function holds the overall classifier logic. Taking the viola jones
// and the hough circles, and returning a vector of candidate regions.
std::vector<Rect> classify(std::vector<Rect> violaCandidates, std::vector<std::vector<int>> houghCandidates)
{
    // convert all hough circles to hough Rects
    std::vector<Rect> houghRects = boundingBoxes(houghCandidates);

    // compute concentric circles.
    std::vector<std::vector<int>> houghConCircles = getConcentricCircles(houghCandidates);
    std::vector<Rect> houghConRects =boundingBoxes(houghConCircles);

    // layer 1
    std::vector<Rect> stage1Filtered =filterOverlapping(violaCandidates,houghRects, 0.8);

    // layer 2
    std::vector<Rect> stage2Filtered = filterOverlapping(stage1Filtered, houghConRects, 0.2);

    // merge any overlapping regions
    std::vector<Rect> out;
    out.push_back(stage2Filtered[0]);
    for (int i =1; i < stage2Filtered.size(); i++)
    {
        bool found = false;
        for (int j = 0; j < out.size(); j++ )
        {
            if (compareRegions(stage2Filtered[i], out[j], 0.5) && !found)
            {
                found = true;
                out[j] = mergeRegions(out[j], stage2Filtered[i]);
            }

        }
        if (!found)
        {
            out.push_back(stage2Filtered[i]);
        }
    }
    return out;

}

// Computes the f1 score across all images
void f1score_all(CascadeClassifier cascade)
{
    string prefix = "test/dart";
    string suffix = ".jpg";
    GroundTruths gt;

    // track metrics across all images
    float tp_total = 0;
    float tpfn_total = 0;
    float tpfp_total = 0;
    std::vector<Rect> groundTruths;

    for (int i = 0; i < 16; i++)
    {
        string imageName= prefix + to_string(i) + suffix;
        Mat image = imread(imageName, CV_LOAD_IMAGE_COLOR);
        groundTruths = gt.getGT(imageName);

        // Get all perspective regions
        std::vector<std::vector<int>> houghCandidates = hough(image, to_string(i) + suffix);
        std::vector<Rect> houghRects= boundingBoxes(houghCandidates);

        std::vector<Rect> violaCandidates = detect(image,cascade);


        std::vector<Rect> candidates = classify(violaCandidates,  houghCandidates);

        for( int i = 0; i < candidates.size(); i++ )
        {
            rectangle(image, Point(candidates[i].x, candidates[i].y),
                    Point(candidates[i].x + candidates[i].width, candidates[i].y +
                        candidates[i].height), Scalar( 147, 20, 255), 2);
        }


        imwrite("results/" + to_string(i) + suffix, image);


        //calculate metrics for a particular image
        float tpfn_image = groundTruths.size();
        float tpfp_image = candidates.size();
        float tp_image = truePositives(groundTruths, candidates);
        float precision_image = tp_image/tpfp_image;
        float recall_image = tp_image/tpfn_image;

        //display metrics for that image
        cout << "image " << i << endl;
        cout << "precision: " << precision_image << endl;
        cout << "recall: " << recall_image << endl;
        cout << "f1 score: " << calculateF1(precision_image, recall_image) << endl;
        cout << "-----------------------" << endl;

        //append to totals for run
        tpfn_total += groundTruths.size();
        tpfp_total += candidates.size();
        tp_total += truePositives(groundTruths, candidates);
    }

    //display overall run metrics.
    float precision = tp_total/tpfp_total;
    float recall = tp_total/tpfn_total;
    cout << "overall precision: " << precision << endl;
    cout << "overall recall: " << recall << endl;
    cout << "overall f1 score: " << calculateF1(precision, recall) << endl;
}



int main( int argc, const char** argv )
{

    String cascade_name = "cascade.xml";
    CascadeClassifier cascade;
	if( !cascade.load( cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

    // if you supply an image as an argument, it will run that img otherwise
    // it will run on all images
    if (argc>1)
    {
        Mat image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
        string s = argv[1] ;
        //get prospective regions
        std::vector<std::vector<int>> houghCandidates = hough(image, s);
        std::vector<Rect> violaCandidates = detect(image, cascade);


        std::vector<Rect> detected = classify(violaCandidates,houghCandidates);
        cout << "the program detected " << detected.size() << " dartboards" << endl;

        //draws the boxes that the classifier detects on to the image
        for( int i = 0; i < detected.size(); i++ )
        {
            rectangle(image, Point(detected[i].x, detected[i].y),
                    Point(detected[i].x + detected[i].width, detected[i].y +
                        detected[i].height), Scalar( 147, 20, 255), 2);
        }

        displayImage(image, "The final detection");
        f1Image(detected, s);
        imwrite( "computed.jpg", image);
    }else {
        f1score_all(cascade);
    }

    return 0;
}
