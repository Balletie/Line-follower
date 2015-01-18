/**
 * Group number: 3
 * Student 1:
 * Skip Lentz, 4334051
 * Student 2:
 * Thomas Baars, 4212134
 */

#include <linefollow/line_detect.h>
#include <vector>
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

  // TODO: Use a more sophisticated way of distinguishing the track.
  cv::threshold(grayscale, track_img, -1, MAXVAL, cv::THRESH_BINARY_INV |
                                                  cv::THRESH_OTSU);
}

void detectLines(cv::InputArray image, cv::Mat& color_edge_img) {
  cv::Mat edge_img;
  cv::Canny(image, edge_img, lowThreshold, lowThreshold * RATIO, 3);
  cv::cvtColor(edge_img, color_edge_img, CV_GRAY2BGR);

  std::vector<cv::Vec4i> lines;
  cv::HoughLinesP(edge_img, lines, 1, CV_PI/180, houghThreshold,
                                                 houghMinLineLength,
                                                 houghMaxLineGap);
  int size = lines.size();
  if(size == 0){
    ROS_INFO("No Lines Detected.");
  } else {

    // Image Width
    int x_size = color_edge_img.cols;

    //Image Height
    int y_size = color_edge_img.rows;

    // Draw center of robovision
    cv::line(color_edge_img, cv::Point(0, 0),
                             cv::Point(x_size, y_size),
                             cv::Scalar(255,0,0), 3, 8);

    // Top-half lines
    int th = 0;
    std::vector<cv::Vec4i> tl(size);
    
    // Closest line
    cv::Vec4i cl;

    for(size_t i = 0; i < size; i++) {
      int x1 = lines[i][0];
      int y1 = lines[i][1];
      int x2 = lines[i][2];
      int y2 = lines[i][3]; 

      // Draw a colored line
      cv::line(color_edge_img, cv::Point(x1, y1),
                               cv::Point(x2, y2),
                               cv::Scalar(0,0,255), 3, 8);      
    }


    // The closest line is cl:
    if(cl[0] == 0){
      ROS_INFO("No closest line");
    }

  }
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
