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

extern int lowThreshold;
extern int houghThreshold;
extern int houghMinLineLength;
extern int houghMaxLineGap;

void distinguishTrack(cv::InputArray, cv::Mat&);
void detectLines(cv::InputArray, cv::Mat&);
void detectRectangles(cv::InputArray, cv::Mat&);
double distance(double, double, double, double);

#endif /*_LINE_DETECT_H_*/
