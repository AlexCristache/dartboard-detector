#include "../headers/f1score.h" 

// evaluates whether the common area is greater than 90% of gTruth and
// how big the candidate area is relative to gTruth
bool rectMatch(cv::Rect gTruth , cv::Rect candidate)
{
    float area = (gTruth & candidate).area();
    if (area > (gTruth.area() * 0.8))
    {
        if( (candidate.area() / gTruth.area()) < 1.3)
        {
            return true;
        }
    }
    return false;
}


// Calculates the true positive rate by seeing if a ground truths overlaps 
// with a candidate, it returns the number that do overlap.  
float truePositives(std::vector<Rect> gTruths, std::vector<Rect> candidates)
{
    float correctCount = 0.0;

    bool match = false;
    int counter = 0;

    for (int i = 0; i < gTruths.size(); i++)
    {
        match = false;
        counter = 0;
        while (!match && counter < candidates.size() )
        {
            if (rectMatch(gTruths[i], candidates[counter]))
            {
                match = true;
                correctCount+=1.0 ;

            }
            counter++;
        }
    }
    return correctCount ;
}


// calculates the true positive rate 
float tpr (std::vector<Rect> gTruths, std::vector<Rect> candidates)
{
   return truePositives(gTruths, candidates)/ gTruths.size() ;
}


// calculates the f1 scorefrom precision and recall 
float calculateF1(float precision, float recall){
    if((recall + precision) == 0)
    {
        return 0;
    }
    return 2 * (precision * recall)/(precision + recall);
}


// Applies a cascade classifier on supplied image and returns a vector of 
// rectangles 
std::vector<Rect> detect(cv::Mat image, CascadeClassifier cascade)
{
    std::vector<Rect> objects;
    Mat grayImage;
    
    //prepare image for detection
    cvtColor(image, grayImage, CV_BGR2GRAY);
    equalizeHist(grayImage, grayImage);
    
    
    // perform viola-jones
    cascade.detectMultiScale( grayImage,objects, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50), Size(500,500) );
    return objects;
}


// Takes the candidate rectanges and an image name from which it then calculates
// the ground truth for that particular image. 
void f1Image(std::vector<Rect> candidates, string imageName)
{
    GroundTruths gt;
    std::vector<Rect> groundTruths = gt.getGT(imageName);
    float tpfn_image = groundTruths.size();
    float tpfp_image = candidates.size();
    float tp_image = truePositives(groundTruths, candidates);
    float precision_image = tp_image/tpfp_image;
    float recall_image = tp_image/tpfn_image;
    cout << "image " << imageName << endl;
    cout << "precision: " << precision_image << endl;
    cout << "recall: " << recall_image << endl;
    cout << "f1 score: " << calculateF1(precision_image, recall_image) << endl;
    cout << "-----------------------" << endl;
}
