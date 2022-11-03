/**
 * @file Motor.cpp
 * @author John Tatum and Modified: M. Good
 * @brief A library to control a motor using the TI DRV8833 dual H-bridge motor driver IC.
 * @version 0.1
 * @date 2022-05-22
 * @copyright Copyright (c) 2022
 * The Arduino DRV8833 %Motor Controller Library is licensed under a <a href="http://opensource.org/licenses/BSD-3-Clause">Revised BSD License</a>. See the included <a href="../../license.txt">license.txt</a> for details. Note that by using this software you agree to the terms of the license.
 * 
 * 
 * @note 
 * Choosing Decay Mode and PWM Frequency - https://learn.adafruit.com/improve-brushed-dc-motor-performance/choosing-decay-mode-and-pwm-frequency
 * How To Change the PWM Frequency Of Arduino Nano - hiles.com/how-to-change-the-pwm-frequency-of-arduino-nano/
 * 
 * most small brushed DC motors will operate nicely with a PWM frequency of 50Hz to 100Hz and slow decay mode.
 * DRV8833 PWM frequency 1 Hz to 50 kHz.
 */

#include <DRV8833.h>

/**
 * @brief Construct a new Motor:: Motor object
 * 
 * @param pinIn1 an integer value representing the first pin used to connect to the motor controller
 * @param pinIn2 an integer value representing the second pin used to connect to the motor controller
 * @param pinStandby an integer value representing the standby pin
 */
Motor::Motor(int pinIn1, int pinIn2, int pinStandby) {
  pin1 = pinIn1;
  pin2 = pinIn2;
  pinEnable = pinStandby;

  #if 1
    // Nano default PWM frequency from D3, D9, D10, and D11 is 490.20 Hz
    //TCCR1B &= B11111000 | B00000101; // PWM 30.64 Hz
    //TCCR1B &= B11111000 | B00000100; // PWM 122.55 Hz
    //TCCR1B &= B11111000 | B00000011; // PWM 490.20 Hz (Default)
    //TCCR1B &= B11111000 | B00000010; // PWM 3921.16 Hz
    //TCCR1B &= B11111000 | B00000001; // PWM 31372.55 Hz

    // Nano default PWM frequency for D5 & D6 is 976.56 Hz
    //TCCR0B &= B11111000 | B00000101; // PWM 61.04 Hz
    //TCCR0B &= B11111000 | B00000100; // PWM 244.14 Hz
    //TCCR0B &= B11111000 | B00000011; // PWM 976.56 Hz (Default)
    //TCCR0B &= B11111000 | B00000010; // PWM 7812.50 Hz
    //TCCR0B &= B11111000 | B00000001; // PWM 62500.00 Hz
  #endif

  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pinEnable, OUTPUT);

  intSpeed = 0;
  isRunning = false;
}

/*I probably need to add a destructor here to call stopMotor(), but I no longer have this motor controller available so I will wait until I have another one so that I can test this change.*/

void Motor::enable(bool state) {
  if(state) {
    digitalWrite(pinEnable, HIGH);
  } else {
    digitalWrite(pinEnable, LOW);
  }
}

/**
 * @brief A method to set the speed of the motor.
 * 
 * @param intIn a value between -100 and 100(inclusive) representing the speed and direction at which the motor will run.
 */
void Motor::setMotorSpeed(int intIn) {
  if ((intIn >= -100) && (intIn <= 100)) {
    intSpeed = int((float(intIn) / 100) * 255);
  }
  else {
    intSpeed = intSpeed;
  }

  /*The following is to change the speed if the motor is already running*/
  if(isRunning) {
    startMotor();
  }
}

/**
 * @brief returns current set speed for a Motor object.
 * 
 * @return a value between -100 and 100(inclusive) representing the speed and direction at which the motor will run.
 */
int Motor::getMotorSpeed() {
  return intSpeed;
}

void Motor::setDecayMode(int decayMode){
  mDecayMode = decayMode;
}

/**
 * @brief returns the current run state of the motor
 * 
 * @return true is running
 * @return false is not running
 */
bool Motor::isMotorRunning() {
  return isRunning;
}

void Motor::stopMotor() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  isRunning = false;
}

void Motor::startMotor() {

  if (intSpeed > 0) {

    if(mDecayMode == DECAY_MODE_FAST) {
      analogWrite(pin1, abs(intSpeed));
      digitalWrite(pin2, LOW);
    } else {
      digitalWrite(pin1, HIGH);
      analogWrite(pin2, abs(intSpeed));
    }
    isRunning = true;

  }	else if (intSpeed < 0) {

    if(mDecayMode == DECAY_MODE_FAST) {
      digitalWrite(pin1, LOW);
      analogWrite(pin2, abs(intSpeed));
    } else {
      analogWrite(pin1, abs(intSpeed));
      digitalWrite(pin2, HIGH);
    }
    isRunning = true;

  }	else {
    stopMotor();
  }
}