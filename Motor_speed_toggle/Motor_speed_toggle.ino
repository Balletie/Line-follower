#include "bluetooth.h"

#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/UInt8.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>

#define REV1 7
#define EN1  24
#define FWD1 6

#define REV2 3
#define EN2  25
#define FWD2 2

#define TRIG 23
#define ECHO 22

#define MAX_VEL 255

ros::NodeHandle_<Bluetooth> nh;

void motorCb( const std_msgs::Empty& toggle_msg){
  digitalWrite(FWD1, HIGH-digitalRead(FWD1));
  digitalWrite(FWD2, HIGH-digitalRead(FWD2));
}

void speedCb(const std_msgs::UInt8& speed_msg){
  analogWrite(FWD1, (int)speed_msg.data);
  analogWrite(FWD2, (int)speed_msg.data);
}

void twistCb(const geometry_msgs::Twist& twist_msg){
  float lin_vel = twist_msg.linear.x;
  float ang_vel = twist_msg.angular.z;
  int left_speed = 0, right_speed = 0;
  left_speed = lin_vel * MAX_VEL;
  right_speed = lin_vel * MAX_VEL;
  left_speed += -ang_vel * MAX_VEL;
  right_speed += ang_vel * MAX_VEL;

  if (left_speed < 0) {
    digitalWrite(FWD1, LOW);
    analogWrite(REV1, left_speed);
  } else {
    digitalWrite(REV1, LOW);
    analogWrite(FWD1, left_speed);
  }
  if (right_speed < 0) {
    digitalWrite(FWD2, LOW);
    analogWrite(REV2, right_speed);
  } else {
    digitalWrite(REV2, LOW);
    analogWrite(FWD2, right_speed);
  }
}

ros::Subscriber<geometry_msgs::Twist> twist("cmd_vel", &twistCb);

ros::Subscriber<std_msgs::Empty> sub("toggle_motor", &motorCb);
ros::Subscriber<std_msgs::UInt8> spd("speed_control", &speedCb);

void setup()
{ 
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(REV1, OUTPUT);
  pinMode(REV2, OUTPUT);
  pinMode(FWD1, OUTPUT);
  pinMode(FWD2, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, HIGH);
  digitalWrite(REV1, LOW);
  digitalWrite(REV2, LOW);
  nh.initNode();
  nh.subscribe(sub);
  nh.subscribe(spd);
  nh.subscribe(twist);
}

long getRange() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  return (duration / 2) / 29.1;
}

void loop()
{  
  nh.spinOnce();

  if (getRange() < 10) {
    digitalWrite(FWD1, LOW);
    digitalWrite(FWD2, LOW);
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
  delay(1);
}

