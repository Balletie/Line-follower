/**
 * Group number: 3
 * Student 1:
 * Skip Lentz, 4334051
 * Student 2:
 * Thomas Baars, 4212134
 */

#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_
#include <ArduinoHardware.h>

class Bluetooth : public ArduinoHardware {
  // Override the constructor
  public : Bluetooth () : ArduinoHardware (&Serial1, 57600) { } ;
};
#endif /*_BLUETOOTH_H_*/
