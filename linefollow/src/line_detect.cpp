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

void distinguishTrack(cv::InputArray image, cv::Mat& track_img) {
  cv::Mat grayscale;
  cv::cvtColor(image, grayscale, CV_BGR2GRAY);

  // TODO: Use a more sophisticated way of distinguishing the track.
  cv::threshold(grayscale, track_img, -1, MAXVAL, cv::THRESH_BINARY_INV |
                                                  cv::THRESH_OTSU);
}
