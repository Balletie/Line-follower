/**
 * Group number: 3
 * Student 1:
 * Skip Lentz, 4334051
 * Student 2:
 * Thomas Baars, 4212134
 */

#ifndef _LINE_DETECT_H_
#define _LINE_DETECT_H_

#include <opencv2/opencv.hpp>
#include <geometry_msgs/Twist.h>

// The values of the line detection parameters are made available to the slider window
extern int lowThreshold;
extern int houghThreshold;
extern int houghMinLineLength;
extern int houghMaxLineGap;

void distinguishTrack(cv::InputArray, cv::Mat&);
geometry_msgs::Twist detectLine(cv::InputArray, cv::Mat&);
double angle(double, double, double, double);
double distance(double, double, double, double);

#endif /*_LINE_DETECT_H_*/
