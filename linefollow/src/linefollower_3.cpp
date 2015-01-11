/**
 * Group number: 3
 * Student 1:
 * Skip Lentz, 4334051
 * Student 2:
 * Thomas Baars, 4212134
 */
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <vector>

void imageCallback(const sensor_msgs::ImageConstPtr& img) {
  try {
    cv::Mat cv_img, edge_img, color_edge_img;
    cv_img = cv_bridge::toCvShare(img, "bgr8")->image;

    cv::Canny(cv_img, edge_img, 50, 200, 3);
    cv::cvtColor(edge_img, color_edge_img, CV_GRAY2BGR);

    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(edge_img, lines, 1, CV_PI/180, 80, 30, 10);
    for(size_t i = 0; i < lines.size(); i++) {
        cv::line(color_edge_img, cv::Point(lines[i][0], lines[i][1]),
                                 cv::Point(lines[i][2], lines[i][3]),
                                 cv::Scalar(0,0,255), 3, 8 );
    }
    cv::imshow("Hello_World", color_edge_img);
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

  cv::namedWindow("Hello_World");
  cv::startWindowThread();
  ros::spin();
  cv::destroyWindow("Hello_World");
}
