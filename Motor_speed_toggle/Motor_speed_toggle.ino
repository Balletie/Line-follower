#include "motor.h"
#include "bluetooth.h"

#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/UInt8.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>

#define TRIG 23
#define ECHO 22

ros::NodeHandle_<Bluetooth> nh;
bool obstacle = false;
unsigned long time;
RobotTank tank;

void motorCb( const std_msgs::Empty& toggle_msg) {
  digitalWrite(FWD1, HIGH-digitalRead(FWD1));
  digitalWrite(FWD2, HIGH-digitalRead(FWD2));
}

void speedCb(const std_msgs::UInt8& speed_msg) {
  analogWrite(FWD1, (int)speed_msg.data);
  analogWrite(FWD2, (int)speed_msg.data);
}

void twistCb(const geometry_msgs::Twist& twist_msg) {
  float lin_vel = twist_msg.linear.x;
  float ang_vel = twist_msg.angular.z;
  tank.setSpeed(lin_vel, ang_vel);
}

ros::Subscriber<geometry_msgs::Twist> twist("cmd_vel", &twistCb);

ros::Subscriber<std_msgs::Empty> sub("toggle_motor", &motorCb);
ros::Subscriber<std_msgs::UInt8> spd("speed_control", &speedCb);

void setup() {
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(REV1, OUTPUT);
  pinMode(REV2, OUTPUT);
  pinMode(FWD1, OUTPUT);
  pinMode(FWD2, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(13, OUTPUT);

  digitalWrite(13, HIGH);
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

void loop() {
  nh.spinOnce();
  unsigned long new_time = millis();
  if (new_time - time >= 40) {
    if (obstacle = (getRange() < 10)) {
      if (tank.enabled){
         tank.toggle();
      }
    }
    time = new_time;
  }
  if (!(obstacle || tank.enabled)) tank.toggle();
  delay(1);
}
