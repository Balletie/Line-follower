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

int lowThreshold = 60;
int houghThreshold = 10;
int houghMinLineLength = 10;
int houghMaxLineGap = 14;

void distinguishTrack(cv::InputArray image, cv::Mat& track_img) {
  cv::Mat grayscale;
  cv::cvtColor(image, grayscale, CV_BGR2GRAY);

  cv::threshold(grayscale, track_img, -1, MAXVAL, cv::THRESH_BINARY_INV |
                                                  cv::THRESH_OTSU);
}

geometry_msgs::Twist detectLine(cv::InputArray image, cv::Mat& color_edge_img) {
  

  cv::Mat edge_img;
  distinguishTrack(image, edge_img);
  cv::Canny(edge_img, edge_img, lowThreshold, lowThreshold * RATIO, 3);
  cv::cvtColor(edge_img, color_edge_img, CV_GRAY2BGR);

  std::vector<cv::Vec4i> lines;
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
    if(d < current_distance && a < 90){
      cl = lines[i];
      current_distance = d;
    }
    // Draw a colored line
    /*cv::line(color_edge_img, cv::Point(x1, y1),
                               cv::Point(x2, y2),
                               cv::Scalar(0,0,255), 3, 8); */     
    }
  }

  // The closest line is cl:
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

      if(distance(x, x2, y, y2) > 100){
        x1 = x2;
        y1 = y2;
        x2 = x;
        y2 = y;
      } 

      angular = angle(x1, x2, y1, y2);
       // Draw a colored line
      cv::line(color_edge_img, cv::Point(cl[0], cl[1]),
                               cv::Point(cl[2], cl[3]),
                               cv::Scalar(0,0,255), 3, 8);
      //cv::circle(color_edge_img, cv::Point(x,y), 100, cv::Scalar(0,255,0), 3, 8);

  }

  geometry_msgs::Twist msg;
  msg.linear.x = linear;
  msg.angular.z = angular / 90;
  return msg;
}

double angle(double x0, double x1, double y0, double y1){
  double y_diff = abs(y1-y0);
  double x_diff = abs(x1-x0);
  int sign = 1;
  if (y1 - y0 > 0) sign = -1;
  //return sign;
  return (sign * (atan(y_diff/x_diff)) * (180 / CV_PI));
}

double distance(double x0, double x1, double y0, double y1){
  return sqrt(((x1-x0)*(x1-x0)) + ((y1-y0)*(y1-y0)));
}

void detectRectangles(cv::InputArray image, cv::Mat& track_img){
  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;
  cv::Mat grayscale;
  cv::cvtColor(image, grayscale, CV_BGR2GRAY);
  cv::threshold(grayscale, track_img, -1, MAXVAL, cv::THRESH_BINARY_INV|cv::THRESH_OTSU);
  cv::adaptiveThreshold(track_img, track_img, 255, cv::ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 3,0);
  cv::findContours(track_img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));
  std::vector<cv::RotatedRect> minRect(contours.size());

  for(int i=0; i < contours.size(); i++){
    minRect[i] = minAreaRect(cv::Mat(contours[i]));
    cv::drawContours(track_img, contours, i, cv::Scalar(255,0,0), 3, 8, hierarchy, 0, cv::Point());
    cv::Point2f rect_points[4];
    minRect[i].points(rect_points);
    for(int j=0; j < 4; j++){
      cv::line(track_img, rect_points[j], rect_points[(j+1)%4], cv::Scalar(255,0,0), 3, 8);
    }
  }
}
