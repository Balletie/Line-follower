#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_
#include <ArduinoHardware.h>

class Bluetooth : public ArduinoHardware {
  public : Bluetooth () : ArduinoHardware (&Serial1, 57600) { } ;
};
#endif /*_BLUETOOTH_H_*/
