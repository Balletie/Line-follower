#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

void imageCallback(const sensor_msgs::ImageConstPtr& img) {
  try{
    cv::imshow("Hello_World", cv_bridge::toCvShare(img, "bgr8")->image);
    cv::waitKey(30);
  } catch (cv_bridge::Exception& e) {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", img->encoding.c_str());
  }
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "image_test");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("camera/image", 1, imageCallback);
  cv::namedWindow("Hello_World");
  cv::startWindowThread();
  ros::spin();
  cv::destroyWindow("Hello_World");
}
