/**
 * Group number: 3
 * Student 1:
 * Skip Lentz, 4334051
 * Student 2:
 * Thomas Baars, 4212134
 */

#include <linefollow/line_detect.h>
#include <vector>
#define RATIO     3
#define MAXVAL    255

int lowThreshold = 50;
int houghThreshold = 80;
int houghMinLineLength = 30;
int houghMaxLineGap = 10;

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
  for(size_t i = 0; i < lines.size(); i++) {
    // Draw a colored line
    cv::line(color_edge_img, cv::Point(lines[i][0], lines[i][1]),
                             cv::Point(lines[i][2], lines[i][3]),
                             cv::Scalar(0,0,255), 3, 8);
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
