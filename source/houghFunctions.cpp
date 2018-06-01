#include "../headers/houghFunctions.h"

//threshold the pixels in an image by setting those higher than
//@value to 255 and the others to 0
void threshold2(cv::Mat image, int value)
{
  // Threshold by looping through all pixels
  for (int y = 0; y<image.rows; y++) {
    for (int x = 0; x<image.cols; x++) {
      uchar pixel = image.at<uchar>(y, x);
      if (pixel > value)
      {
        image.at<uchar>(y, x) = 255;
      }
      else {
        image.at<uchar>(y, x) = 0;
      }
    }
  }
}

//calculates the euclidean distance between 2 points
float distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow((x1-x2),2)+pow((y1-y2),2));
}


//populate the 3D array with the votes for each pixel to be the center of a circle
std::vector<Mat> generate_accumulator(Mat grad, int min_radius, int max_radius, Mat theta, int step)
{
  int cols = grad.cols;
  int rows = grad.rows;
  int r_boundary = (max_radius - min_radius)/step;
  std::vector<Mat> accumulator;

  for (int r = 0; r < r_boundary; r++)
  {
    accumulator.push_back(Mat::zeros(rows,cols,CV_32S));
  }

  for (int x = 0; x < rows; x++)
  {
    for (int y = 0; y < cols; y++)
    {
      if (grad.at<uchar>(x,y) == 255)
      {
        for(int r = 0; r < r_boundary; r++)
        {
          //calculate the radius of each possible circle
          int radius = r * step + min_radius;

          int a2 = x + (radius * cos(theta.at<float>(x, y)));
          int b2 = y + (radius * sin(theta.at<float>(x, y)));

          if (a2 > 0 &&  b2 > 0 && a2 < rows && b2 < cols)
          {
            accumulator[r].at<int>(a2,b2) +=1;
          }
          int a1 = x - (radius * cos(theta.at<float>(x, y)));
          int b1 = y - (radius * sin(theta.at<float>(x, y)));

          if (a1 > 0 &&  b1 > 0 && a1 < rows && b1 < cols)
          {
            accumulator[r].at<int>(a1,b1) +=1;
          }
        }
      }

    }
  }
  return accumulator;
}


//generate the Hough space from the 3D array of votes
Mat houghSpace(std::vector<Mat> accumulator)
{
  int rows = accumulator[0].rows;
  int cols = accumulator[0].cols;
  int r_boundary = accumulator.size();
  Mat output = Mat::zeros(rows,cols, CV_32S);
  for(int x = 0; x < rows; x++)
  {
    for (int y = 0; y < cols; y++)
    {
      int total = 0;
      for (int r = 0; r < r_boundary; r++)
      {
        total += accumulator[r].at<int>(x,y);
      }
      output.at<int>(x,y) = total;
    }
  }

  output.convertTo(output, CV_8UC1);

  return output;
}

//iterate through the 3D array of votes and only keep the circles with a score bigger than @circle_threshold
std::vector<std::vector<int>> thresholdCircles(std::vector<Mat> accumulator, int circle_threshold, int min_radius, int step)
{
  int r_boundary  = accumulator.size();
  int rows        = accumulator[0].rows;
  int cols        = accumulator[0].cols;
  std::vector<std::vector<int>> relevantCircles;

  for(int x = 0; x < rows; x++) {
    for(int y = 0; y < cols; y++) {
      for(int r = 0; r < r_boundary; r++) {
        if(accumulator[r].at<int>(x, y) > circle_threshold) {
          int radius = r * step + min_radius;
          std::vector<int> tempCoordinates;
          tempCoordinates.push_back(x);
          tempCoordinates.push_back(y);
          tempCoordinates.push_back(radius);
          relevantCircles.push_back(tempCoordinates);
        }
      }
    }
  }

  return relevantCircles;
}

//check if a point is close enough to the center of another already found circle
int contains(std::vector<std::vector<int>> circles, int x, int y)
{
  for(int i = 0; i < circles.size(); i++) {
    int x_circle = circles[i][0];
    int y_circle = circles[i][1];
    if(distance(x,y, x_circle, y_circle) < 50) {
      return i;
    }
  }
  return -1;
}


//filter the thresholded circles to display only one circle for a cluster of centers
std::vector<std::vector<int>> getConcentricCircles(std::vector<std::vector<int>> relevantCircles)
{
  std::vector<std::vector<int>> concentricCircles;
  for(int i = 0; i < relevantCircles.size(); i++) {
    for(int j = i + 1; j < relevantCircles.size(); j++) {
      int xI = relevantCircles[i][0];
      int yI = relevantCircles[i][1];

      int xJ = relevantCircles[j][0];
      int yJ = relevantCircles[j][1];

      if(distance(xI, yI, xJ, yJ) < 50) {
        int position = contains(concentricCircles, xI, yI);
        if(position > -1) {
          int radius = concentricCircles[position][2];
          if(relevantCircles[i][2] > radius) {
            int avg_radius = (radius + relevantCircles[i][2])/2;
            concentricCircles[position][2] = avg_radius;
          }
        }
        else {
          std::vector<int> temp;
          temp.push_back(xI);
          temp.push_back(yI);
          temp.push_back(relevantCircles[i][2]);
          concentricCircles.push_back(temp);
        }
      }
    }
  }

  return concentricCircles;
}

//function that draws circles on a given image
void drawHoughCircles(Mat image, std::vector<std::vector<int>> relevantCircles, std::vector<Mat> accumulator)
{
  int numberOfCircles = relevantCircles.size();
  for(int i = 0; i < numberOfCircles; i++) {
    int x       = relevantCircles[i][0];
    int y       = relevantCircles[i][1];
    int radius  = relevantCircles[i][2];
    circle(image,Point(y, x), radius, Scalar(255,129,0), 2, LINE_MAX);
    circle(image,Point(y, x), 2, Scalar(255,129,0), 2, LINE_MAX);
  }

  displayImage(image, "Circles");
}


//calculate the minimum threshold for a circle to be considered viable by
//only considering the upper @percentage of values in the 3D array
int getThreshold(std::vector<Mat> accumulator, float percentage)
{
  int r_depth = accumulator.size();
  int rows = accumulator[0].rows;
  int cols = accumulator[0].cols;
  int max = 0;
  int min = 20000;
  std::vector<int> vote_values;
  std::vector<int>::iterator position;
  int i = 0;
  for(int x = 0; x < rows; x++) {
    for(int y = 0; y < cols; y++) {
      for(int r = 0; r < r_depth; r++) {
        int acc_value = accumulator[r].at<int>(x, y);
        if(acc_value > max) {
          max = acc_value;
        }
        if(acc_value < min) {
          min = acc_value;
        }
        if(accumulator[r].at<int>(x, y) > 0) {
          position = find(vote_values.begin(), vote_values.end(), acc_value);
          if(position == vote_values.end()) {
            vote_values.push_back(acc_value);
          }
        }
      }
    }
  }
  int thr_position = percentage * vote_values.size();
  nth_element(vote_values.begin(), vote_values.end() - thr_position, vote_values.end());
  thr_position = vote_values[vote_values.size() * (1 - percentage)];

  return thr_position;
}


//perform the hough transform
std::vector<std::vector<int>> hough(Mat src, string imageName)
{
    Mat src_gray;
    Mat grad;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

    /// Convert it to gray
    cvtColor( src, src_gray, CV_BGR2GRAY );

    /// Generate grad_x and grad_y
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    /// Gradient X
    Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );

    /// Gradient Y
    Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );

    /// Total Gradient (approximate)
    addWeighted( abs_grad_x, 0.8, abs_grad_y, 0.8, 0, grad );

    Mat theta = Mat(src_gray.rows, src_gray.cols, CV_64F);
    for(int x = 0; x < theta.rows; x++) {
      for(int y = 0; y < theta.cols; y++) {
        theta.at<float>(x, y) = atan2(abs_grad_x.at<uchar>(x, y), abs_grad_y.at<uchar>(x, y));
      }
    }

    int thr = 0;

    //calculate the total sum of the pixels in the image
    for(int x = 0; x < grad.rows; x++) {
      for(int y = 0; y < grad.cols; y++) {
        thr += grad.at<uchar>(x, y);
      }
    }
    //take the average of it in order to threshold the gradient
    thr = thr/(grad.rows * grad.cols);

    threshold2(grad, thr);


    //displayImage(grad, "gradient");

    int min_radius = 10;
    int max_radius = sqrt(pow(grad.cols, 2) + pow(grad.rows, 2))/2;
    int step = 1;

    std::vector<Mat> accumulator = generate_accumulator(grad, min_radius, max_radius, theta, step);
    Mat hough_space_image = houghSpace(accumulator);
    int circle_threshold = getThreshold(accumulator, 0.15);
    std::vector<std::vector<int>> relevantCircles = thresholdCircles(accumulator, circle_threshold, min_radius, step);

    imwrite("gradient_images/gradient_" + imageName, grad);
    imwrite("hough_spaces/hough_space_" + imageName, hough_space_image);

    //std::vector<std::vector<int>> concentricCircles = getConcentricCircles(relevantCircles);
    //displayImage(hough_space_image, "Hough Space");
    //drawHoughCircles(src, relevantCircles, accumulator);


	return relevantCircles;
}
