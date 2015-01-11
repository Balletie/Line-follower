/**
 * Group number: 3
 * Student 1:
 * Skip Lentz, 4334051
 * Student 2:
 * Thomas Baars, 4212134
 */

#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <ros/ros.h>

#include "linefollow/line_detect.h"

#define WINDOW_NAME "Hello_World"

void imageCallback(const sensor_msgs::ImageConstPtr& img) {
  try {
    cv::Mat cv_img, color_edge_img;
    cv_img = cv_bridge::toCvShare(img, "bgr8")->image;

    detectTrack(cv_img, color_edge_img);

    cv::imshow(WINDOW_NAME, color_edge_img);
    cv::waitKey(30);
  } catch (cv_bridge::Exception& e) {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", img->encoding.c_str());
  }
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "image_test");
  ros::NodeHandle nh;

  image_transport::ImageTransport it(nh);
  image_transport::TransportHints hints("compressed");
  image_transport::Subscriber sub = it.subscribe("camera/image", 1,
                                                 imageCallback, hints);

  cv::namedWindow(WINDOW_NAME);
  cv::startWindowThread();
  ros::spin();
  cv::destroyWindow(WINDOW_NAME);
}
