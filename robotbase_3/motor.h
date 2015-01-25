/**
 * Group number: 3
 * Student 1:
 * Skip Lentz, 4334051
 * Student 2:
 * Thomas Baars, 4212134
 */

#ifndef _MOTOR_H_
#define _MOTOR_H_

// Define the pins of the motors
#define REV1 7
#define EN1  24
#define FWD1 6

#define REV2 3
#define EN2  25
#define FWD2 2

// The maximum speed
#define MAX_VEL 255

class Motor {
  public:
  Motor(int _en, int _fwd, int _rev) : en_pin(_en),
                                       fwd_pin(_fwd),
                                       rev_pin(_rev),
                                       m_speed(0)
  {}
  void setSpeed(float speed);
  void setEnabled(bool enabled);
  private:
  int en_pin;
  int fwd_pin;
  int rev_pin;
  int m_speed;
};

class RobotTank {
  public:
  RobotTank() : left(EN1, FWD1, REV1),
                right(EN2, FWD2, REV2),
                enabled(true)
  {}
  void setSpeed(float linear, float angular);
  void toggle();

  bool enabled;
  private:
  Motor left, right;
};

#endif /*_MOTOR_H_*/
