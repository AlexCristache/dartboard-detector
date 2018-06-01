#include "../headers/utility.h" 


// takes an image and image name, and displays it in a window 
void displayImage(cv::Mat image, string name)
{
    namedWindow(name, CV_WINDOW_AUTOSIZE) ;
    imshow(name, image);
    waitKey(0);
}


// Draws the ground truth regions and candidates region on the supplied image
void visualise(cv::Mat image, std::vector<Rect> gTruth, std::vector<Rect> candidates, string imageName)
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
    imwrite( "f1_out/" + imageName, image );
    displayImage(image, "visualisation");
}

