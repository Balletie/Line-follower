/**
 * Group number: 3
 * Student 1:
 * Skip Lentz, 4334051
 * Student 2:
 * Thomas Baars, 4212134
 */

#include <linefollow/line_detect.h>
#include <vector>
#define THRESHOLD 127
#define MAXVAL    255

void detectTrack(cv::InputArray image, cv::Mat& track_img) {
  cv::Mat grayscale;
  cv::cvtColor(image, grayscale, CV_BGR2GRAY);
  // TODO: Use a more sophisticated way of distinguishing the track.
  cv::threshold(grayscale, track_img, THRESHOLD, MAXVAL, cv::THRESH_BINARY_INV);
}

void detectLines(cv::InputArray image, cv::Mat& color_edge_img) {
  cv::Mat edge_img;
  cv::Canny(image, edge_img, 50, 200, 3);
  cv::cvtColor(edge_img, color_edge_img, CV_GRAY2BGR);

  std::vector<cv::Vec4i> lines;
  cv::HoughLinesP(edge_img, lines, 1, CV_PI/180, 80, 30, 10);
  for(size_t i = 0; i < lines.size(); i++) {
    // Draw a colored line
    cv::line(color_edge_img, cv::Point(lines[i][0], lines[i][1]),
                             cv::Point(lines[i][2], lines[i][3]),
                             cv::Scalar(0,0,255), 3, 8);
  }
}
