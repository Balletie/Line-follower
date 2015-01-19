#include "motor.h"
#include "bluetooth.h"

#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/UInt8.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>

#define TRIG 23
#define ECHO 22
#define LED 13
#define FAIL_SAFE_T 2000

ros::NodeHandle_<Bluetooth> nh;
bool obstacle = false;
bool timedout = false;
unsigned long ob_time;
unsigned long sg_time;
RobotTank tank;

void twistCb(const geometry_msgs::Twist& twist_msg) {
  if (timedout && !obstacle) {
    tank.toggle();
    timedout = false;
  }
  sg_time = millis();

  float lin_vel = twist_msg.linear.x;
  float ang_vel = twist_msg.angular.z;
  tank.setSpeed(lin_vel, ang_vel);
}

ros::Subscriber<geometry_msgs::Twist> twist("cmd_vel", &twistCb);

void setup() {
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(REV1, OUTPUT);
  pinMode(REV2, OUTPUT);
  pinMode(FWD1, OUTPUT);
  pinMode(FWD2, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(LED, LOW);
  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, HIGH);
  digitalWrite(REV1, LOW);
  digitalWrite(REV2, LOW);

  nh.initNode();
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
  if (new_time - ob_time >= 40) {
    if (obstacle = (getRange() < 10)) {
      if (tank.enabled){
         tank.toggle();
      }
    }
    ob_time = new_time;
  }
  if (!timedout && new_time - sg_time >= FAIL_SAFE_T) {
    if (tank.enabled) {
      tank.toggle();
    }
    timedout = true;
  }
  if (!(obstacle || timedout || tank.enabled)) tank.toggle();
  delay(1);
}
