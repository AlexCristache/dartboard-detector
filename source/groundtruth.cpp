#include "../headers/groundtruth.h"
#include "string.h"
#include <fstream> 

// takes a string name, which is used to access the relevant ground truth file
// stored in gt_files directory on the root level of the project.
// It then reads the file line by line and pushes it into a vector. 
std::vector<Rect> GroundTruths::getGT(string s)
{
    std::vector<Rect> gt; 
    int length = s.length();
    int startPos = 0 + 5;
    int finishPos = length - 4;
    string prefix = "gt_files/"; 
    string extension = ".txt" ; 
    
    // parse image name to obtain just the image name, and not include "test/"
    string out = "";
    for(int i = startPos; i < finishPos; i++)
    {
      out += s[i];
    }
    out = prefix + out + extension; 

    string line;
    
    
    ifstream gtFile(out);
    int i ;  
    if (gtFile.is_open())
    {
        while ( getline (gtFile,line) )
        {
            stringstream ss(line); 
            // values stores each x or y value in the co-ordinates 
            int values[4] ;  
            int j = 0; 

            while(ss >> i)
            {
                values[j] = i; 
                j++;             
                // check the next character in the string without extracting it.  
                if (ss.peek() == ',')
                {
                    ss.ignore() ; 
                }
            }
            
            // append the array to a vector of rectangles 
            gt.push_back(Rect(Point(values[0],values[1]), Point(values[2],values[3])));
        }
        gtFile.close();
    } else 
    {
        cout << "Unable to open file"; 
    }  
    return gt ; 
}


// loads the ground truth for face detections in image 4. This is hard coded.  
std::vector<Rect> GroundTruths::get4()
{
    std::vector<Rect> groundTruth4;
    groundTruth4.push_back(Rect(Point(351, 110), Point(472, 268)));
    return groundTruth4;
}


// loads the ground truth for face detections in image 5. This is hard coded.  
std::vector<Rect> GroundTruths::get5()
{
    std::vector<Rect> groundTruth5;
    groundTruth5.push_back(Rect(Point( 57, 250), Point(117, 322)));
    groundTruth5.push_back(Rect(Point( 70, 147), Point(119, 203)));
    groundTruth5.push_back(Rect(Point(197, 215), Point(248, 282)));
    groundTruth5.push_back(Rect(Point(253, 172), Point(302, 230)));
    groundTruth5.push_back(Rect(Point(295, 240), Point(345, 310)));
    groundTruth5.push_back(Rect(Point(381, 194), Point(437, 236)));
    groundTruth5.push_back(Rect(Point(430, 237), Point(482, 297)));
    groundTruth5.push_back(Rect(Point(517, 180), Point(570, 239)));
    groundTruth5.push_back(Rect(Point(564, 240), Point(615, 314)));
    groundTruth5.push_back(Rect(Point(684, 244), Point(731, 309)));
    groundTruth5.push_back(Rect(Point(650, 188), Point(702, 247)));
    return groundTruth5;
}


// loads the ground truth for face detections in image 13. This is hard coded.  
std::vector<Rect> GroundTruths::get13()
{
    std::vector<Rect> groundTruth13;
    groundTruth13.push_back(Rect(Point(425, 117), Point(523, 258)));

    return groundTruth13;
}


// loads the ground truth for face detections in image 14. This is hard coded.  
std::vector<Rect> GroundTruths::get14()
{
    std::vector<Rect> groundTruth14;
    groundTruth14.push_back(Rect(Point(551, 321), Point(469, 210)));
    groundTruth14.push_back(Rect(Point(726, 118), Point(823, 285)));
    return groundTruth14;
}
// loads the ground truth for face detections in image 15. This is hard coded.  
std::vector<Rect> GroundTruths::get15()
{
    std::vector<Rect>  groundTruth15;
    groundTruth15.push_back (Rect(Point( 69, 135), Point(125, 216)));
    groundTruth15.push_back (Rect(Point(376, 114), Point(419, 188)));
    groundTruth15.push_back (Rect(Point(543, 124), Point(596, 211)));
    return groundTruth15;
}
