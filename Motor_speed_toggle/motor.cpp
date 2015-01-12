#include "motor.h"
#include "Arduino.h"

void Motor::setSpeed(float speed) {
  m_speed = speed * MAX_VEL;
  if (m_speed < 0) {
    if (m_speed < -MAX_VEL) m_speed = -MAX_VEL;
    digitalWrite(fwd_pin, LOW);
    analogWrite(rev_pin, -m_speed);
  } else {
    if (m_speed > MAX_VEL) m_speed = MAX_VEL;
    digitalWrite(rev_pin, LOW);
    analogWrite(fwd_pin, m_speed);
  }
}

void Motor::setEnabled(bool enabled) {
  if (enabled)  digitalWrite(en_pin, HIGH);
  else          digitalWrite(en_pin, LOW);
}

void RobotTank::setSpeed(float linear, float angular) {
  left.setSpeed(linear - angular);
  right.setSpeed(linear + angular);
}

void RobotTank::toggle() {
  enabled = !enabled;
  left.setEnabled(enabled);
  right.setEnabled(enabled);
}
