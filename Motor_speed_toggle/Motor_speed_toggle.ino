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

ros::NodeHandle_<Bluetooth> nh;

void motorCb( const std_msgs::Empty& toggle_msg){
  digitalWrite(FWD1, HIGH-digitalRead(FWD1));
  digitalWrite(FWD2, HIGH-digitalRead(FWD2));
}

void speedCb(const std_msgs::UInt8& speed_msg){
  analogWrite(FWD1, (int)speed_msg.data);
  analogWrite(FWD2, (int)speed_msg.data);
}

/*
void twistCb(const geometry_msgs::Twist& twist_msg){
  twist_msgs.linear
}

ros::Subscriber<geometry_msgs::Twist> 
*/
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
  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, HIGH);
  digitalWrite(REV1, LOW);
  digitalWrite(REV2, LOW);
  nh.initNode();
  nh.subscribe(sub);
  nh.subscribe(spd);
}

void loop()
{  
  nh.spinOnce();
  delay(1);
}

