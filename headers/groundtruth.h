#ifndef GROUNDTRUTH_H
#define GROUNDTRUTH_H
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

// ground truth class which provides methods to access the gruond truth. 
class GroundTruths
{
    public:
        // get the gruond truth for dartboards in an image named s.
        std::vector<Rect> getGT(string s);

        // get the ground truth for faces in image 4. 
        std::vector<Rect> get4();

        // get the ground truth for faces in image 5. 
        std::vector<Rect> get5();

        // get the ground truth for faces in image 13. 
        std::vector<Rect> get13();

        // get the ground truth for faces in image 14. 
        std::vector<Rect> get14();

        // get the ground truth for faces in image 15.  
        std::vector<Rect> get15();

};

#endif 
