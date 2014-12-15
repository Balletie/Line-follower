/* 
 * rosserial Subscriber Example
 * Blinks an LED on callback
 */

#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/UInt8.h>

ros::NodeHandle  nh;

void motorCb( const std_msgs::Empty& toggle_msg){
  digitalWrite(6, HIGH-digitalRead(6));
  digitalWrite(2, HIGH-digitalRead(2));
}

void speedCb(const std_msgs::UInt8& speed_msg){
  analogWrite(2, (int)speed_msg.data);
  analogWrite(6, (int)speed_msg.data);
}
ros::Subscriber<std_msgs::Empty> sub("toggle_motor", &motorCb);
ros::Subscriber<std_msgs::UInt8> spd("speed_control", &speedCb);

void setup()
{ 
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(24, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(7, LOW);
  nh.initNode();
  nh.subscribe(sub);
  nh.subscribe(spd);
}

void loop()
{  
  nh.spinOnce();
  delay(1);
}

