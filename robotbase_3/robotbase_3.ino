/**
 * Group number: 3
 * Student 1:
 * Skip Lentz, 4334051
 * Student 2:
 * Thomas Baars, 4212134
 */
 
#include "motor.h"
#include "bluetooth.h"
#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/UInt8.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>

// Define the pins
#define TRIG 23
#define ECHO 22
#define LED 13

// The time-out in milliseconds
#define FAIL_SAFE_T 2000

ros::NodeHandle_<Bluetooth> nh;
bool obstacle = false;
bool timedout = false;
unsigned long ob_time;
unsigned long sg_time;
RobotTank tank;

void twistCb(const geometry_msgs::Twist& twist_msg) {
  // If a signal has been received and there is no obstacle then start the tank
  if (timedout && !obstacle) {
    tank.toggle();
    timedout = false;
  }
  // Update the time since a signal has been received
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

  // Ensure all pins are set to their defaults
  digitalWrite(LED, LOW);
  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, HIGH);
  digitalWrite(REV1, LOW);
  digitalWrite(REV2, LOW);

  nh.initNode();
  nh.subscribe(twist);
}

long getRange() {
  // Send an ultrasonic pulse 
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  // Get the response time of the echo
  long duration = pulseIn(ECHO, HIGH);

  // Return the distance in centimeters
  return (duration / 2) / 29.1;
}

void loop() {
  nh.spinOnce();
  unsigned long new_time = millis();
  // Check for obstacles every 40 milliseconds
  if (new_time - ob_time >= 40) {
    // If an object is detected within a radius of 10 centimeters
    if (obstacle = (getRange() < 10)) {
      if (tank.enabled){
         // Stop the tank
         tank.toggle();
      }
    }
    // Update the time
    ob_time = new_time;
  }
  
  // If no message has been received in FAIL_SAFE_T milliseconds then stop the tank
  if (!timedout && new_time - sg_time >= FAIL_SAFE_T) {
    if (tank.enabled) {
      tank.toggle();
    }
    timedout = true;
  }
  
  // Enable the tank if there is no obstacle and it has received a message and the tank was disabled 
  if (!(obstacle || timedout || tank.enabled)) tank.toggle();
  
  delay(1);
}
