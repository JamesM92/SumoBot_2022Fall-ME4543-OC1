/*
Program:			Motor Library

File:				Motor.h

Author:				John Tatum

Notes:				None
*/
#ifndef DRV8833_h
#define DRV8833_h

#include <Arduino.h>
//#include <WProgram.h>

enum motorDecayMode {
  DECAY_MODE_SLOW,      // 
  DECAY_MODE_FAST       // 
};

class Motor {
  private:
  int intSpeed;
  int mDecayMode = DECAY_MODE_FAST;
  int pin1;
  int pin2;
  int pinEnable;
  bool isRunning;

  public:
  Motor(int pinIn1, int pinIn2, int pinStandby);
  void enable(bool state);
  void setMotorSpeed(int intIn);
  void setDecayMode(int decayMode);
  int getMotorSpeed();
  bool isMotorRunning();
  void stopMotor();
  void startMotor();
};
#endif