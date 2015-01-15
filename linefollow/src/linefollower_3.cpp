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
#include <geometry_msgs/Twist.h>
#include <linefollow/line_detect.h>

#define WINDOW_NAME "Hello_World"
#define SLIDER_NAME "Constants slider"

class RosLineFollower{
public:
  RosLineFollower(){
    vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
    image_transport::ImageTransport it(nh);
    image_transport::TransportHints hints("compressed");
    img_sub = it.subscribe("camera/image", 1, &RosLineFollower::imageCallback, this, hints);
  }

  void imageCallback(const sensor_msgs::ImageConstPtr& img) {
    try {
      cv::Mat cv_img, color_edge_img;
      cv_img = cv_bridge::toCvShare(img, "bgr8")->image;

      detectLines(cv_img, color_edge_img);

      //distinguishTrack(cv_img, color_edge_img);
      //detectLines(cv_img, color_edge_img);
      //detectRectangles(cv_img, color_edge_img);

      cv::imshow(WINDOW_NAME, color_edge_img);
      geometry_msgs::Twist msg;
      msg.linear.x = 10;
      msg.angular.z = 5;
      vel_pub.publish(msg);
    } catch (cv_bridge::Exception& e) {
      ROS_ERROR("Could not convert from '%s' to 'bgr8'.", img->encoding.c_str());
    }
  }

private:
  ros::NodeHandle nh;
  ros::Publisher vel_pub;
  image_transport::Subscriber img_sub;
};

int main(int argc, char** argv) {
  ros::init(argc, argv, "line_follower");
  RosLineFollower linefollower;

  cv::namedWindow(WINDOW_NAME);
  cv::namedWindow(SLIDER_NAME);
  cv::createTrackbar("Canny low threshold:", SLIDER_NAME, &lowThreshold, 100);
  cv::createTrackbar("Hough threshold:", SLIDER_NAME, &houghThreshold, 200);
  cv::createTrackbar("Hough min. line:", SLIDER_NAME, &houghMinLineLength, 100);
  cv::createTrackbar("Hough max. gap:", SLIDER_NAME, &houghMaxLineGap, 100);
  cv::startWindowThread();

#if 1
  cv::Mat img;
  cv_bridge::CvImage stupid;
  printf(IMG_PATH);
  img = cv::imread(IMG_PATH);
  stupid.encoding = "bgr8";
  stupid.image = img;

  while(true) linefollower.imageCallback(stupid.toImageMsg());
#endif

  ros::spin();
  cv::destroyWindow(WINDOW_NAME);
}
