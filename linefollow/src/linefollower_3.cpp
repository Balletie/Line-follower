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

#define TEST_FLAG 1
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

  #if TEST_FLAG
  ROS_INFO("Test");
  cv::Mat img;
  cv_bridge::CvImage msg;
  img = cv::imread(IMG_PATH);
  msg.encoding = "bgr8";
  msg.image = img;

  while(true) {
    linefollower.imageCallback(msg.toImageMsg());
    if (cv::waitKey(40) == 27) {
      cv::destroyAllWindows();
      exit(0);
    }
  }
  #else
  ros::spin();
  cv::destroyAllWindows();
  #endif
}
