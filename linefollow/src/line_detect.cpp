/**
 * Group number: 3
 * Student 1:
 * Skip Lentz, 4334051
 * Student 2:
 * Thomas Baars, 4212134
 */

#include <linefollow/line_detect.h>
#include <vector>
#include <cmath>
#include <geometry_msgs/Twist.h>
#include <ros/ros.h>
#define RATIO     3
#define MAXVAL    255

//Default values for line detection parameters
int lowThreshold = 60;
int houghThreshold = 10;
int houghMinLineLength = 10;
int houghMaxLineGap = 14;

void distinguishTrack(cv::InputArray image, cv::Mat& track_img) {
  cv::Mat grayscale;
  cv::cvtColor(image, grayscale, CV_BGR2GRAY);
  // Add THRESH_OTSU to the bit field using OR determines the best threshold to be used 
  cv::threshold(grayscale, track_img, -1, MAXVAL, cv::THRESH_BINARY_INV |
                                                  cv::THRESH_OTSU);
}

geometry_msgs::Twist detectLine(cv::InputArray image, cv::Mat& color_edge_img) {
  cv::Mat edge_img;
  distinguishTrack(image, edge_img);
  // Using the Canny algorithm to detect the edges of the distinguished track
  cv::Canny(edge_img, edge_img, lowThreshold, lowThreshold * RATIO, 3);
  cv::cvtColor(edge_img, color_edge_img, CV_GRAY2BGR);

  std::vector<cv::Vec4i> lines;
  // Using Hough Lines algorithm to generate an array of lines based on the edges detected
  cv::HoughLinesP(edge_img, lines, 1, CV_PI/180, houghThreshold,
                                                 houghMinLineLength,
                                                 houghMaxLineGap);
  int size = lines.size();
  
  // Image Width
  double x_size = color_edge_img.cols;
  //Image Height
  double y_size = color_edge_img.rows;
  
  // Robot Origin
  double x = x_size;
  double y = y_size / 2;

  // Closest line
  cv::Vec4i cl;

  if(size == 0){
    ROS_INFO("No Lines Detected.");
  } else {
    double current_distance = x_size * x_size;
  for(size_t i = 0; i < size; i++) {
    double x1 = lines[i][0];
    double y1 = lines[i][1];
    double x2 = lines[i][2];
    double y2 = lines[i][3]; 

    double d = distance(x,x2,y,y2);
    double a = angle(x1, x2, y1, y2);
    // If the distance to the robot origin is less and the angle is acute
	if(d < current_distance && a < 90){
      // update the closest line
	  cl = lines[i];
      current_distance = d;
    }
    }
  }

  // The linear is a constant 0.5 unless no line is detected
  double linear = 0.5;
  double angular = 0;
  if(cl[0] == 0) {
    ROS_INFO("No closest line");
    linear = 0;
  } else {
      double x1 = cl[0];
      double y1 = cl[1];
      double x2 = cl[2];
      double y2 = cl[3];

	  // If the closest line is outside a 100 unit radius
      if(distance(x, x2, y, y2) > 100){
        // Go to the start of the line segment from the robot origin
		x1 = x2;
        y1 = y2;
        x2 = x;
        y2 = y;
      } 

      angular = angle(x1, x2, y1, y2);
       // Draw the chosen line
      cv::line(color_edge_img, cv::Point(cl[0], cl[1]),
                               cv::Point(cl[2], cl[3]),
                               cv::Scalar(0,0,255), 3, 8);
  }

  geometry_msgs::Twist msg;
  msg.linear.x = linear;
  // Cap the angle to reduce over-correcting
  msg.angular.z = angular / 90;
  return msg;
}

double angle(double x0, double x1, double y0, double y1){
  double y_diff = abs(y1-y0);
  double x_diff = abs(x1-x0);
  int sign = 1;
  // If the slope is positive the angular velocity has to be negative so that the robot goes right 
  if (y1 - y0 > 0) sign = -1;
  return (sign * (atan(y_diff/x_diff)) * (180 / CV_PI));
}

double distance(double x0, double x1, double y0, double y1){
  return sqrt(((x1-x0)*(x1-x0)) + ((y1-y0)*(y1-y0)));
}

