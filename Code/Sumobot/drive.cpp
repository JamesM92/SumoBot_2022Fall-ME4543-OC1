/**
 * @file drive.cpp
 * @author M. Good
 * @brief DRIVE MOTOR
 * @version 0.1
 * @date 2022-06-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <Arduino.h>
#include "drive.h"

motor motor_right; // right side motor
motor motor_left; // left side motor

int drive_state = DRIVE_UNDEFINED;
int drive_state_old = DRIVE_UNDEFINED;

/**
 * @brief Set the up motors object
 * @note 
 * Choosing Decay Mode and PWM Frequency - https://learn.adafruit.com/improve-brushed-dc-motor-performance/choosing-decay-mode-and-pwm-frequency
 * How To Change the PWM Frequency Of Arduino Nano - hiles.com/how-to-change-the-pwm-frequency-of-arduino-nano/
 * 
 */
void drive_setup(void) {
  motor_right.id = char('A');
  motor_right.pin_in1 = PIN_MOTOR_AIN1;
  motor_right.pin_in2 = PIN_MOTOR_AIN2;
  motor_right.mode = MOTOR_FAST_DECAY;
  motor_right.state = MOTOR_COAST;
  motor_right.speed = 0;

  motor_left.id = char('B');
  motor_left.pin_in1 = PIN_MOTOR_BIN1;
  motor_left.pin_in2 = PIN_MOTOR_BIN2;
  motor_left.mode = MOTOR_FAST_DECAY; 
  motor_left.state = MOTOR_COAST;
  motor_left.speed = 0;

  drive_set_enabled(true);

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
}

bool drive_get_enabled(void) {
  return digitalRead(PIN_MOTOR_STBY);
}

void drive_set_enabled(bool state) {
  digitalWrite(PIN_MOTOR_STBY, state);
}

int drive_get_state(void) {
  return drive_state;
}

void drive_update_state(int state) {
  drive_state = state;
  
  #ifdef DRIVE_REPORT_STATE_CHANGES
    if(drive_report_state_change()) Serial.println();
  #endif

}

bool drive_report_state_change(void) {
  // report state change.
  if(drive_state != drive_state_old) {
    drive_state_old = drive_state;
    drive_report_state(drive_state);
    return true;
  } else {
    return false;
  }
}

void drive_report_state(int state) {
  Serial.print(F(" DRIVE_"));
  switch(state) {
    case DRIVE_UNDEFINED: Serial.print(F("UNDEFINED")); break;
    case DRIVE_BRAKE: Serial.print(F("BRAKE")); break;
    case DRIVE_COAST: Serial.print(F("COAST")); break;
    case DRIVE_FORWARD: Serial.print(F("FORWARD")); break;
    case DRIVE_BACKWARD: Serial.print(F("BACKWARD")); break;
    case DRIVE_PIVOT_CCW: Serial.print(F("PIVOT_CCW")); break;
    case DRIVE_PIVOT_CW: Serial.print(F("PIVOT_CW")); break;
    case DRIVE_STEER_CW: Serial.print(F("STEER_CW")); break;
    case DRIVE_STEER_CCW: Serial.print(F("STEER_CCW")); break;
  }
}

//--------------------------------------------
void drive_brake(void) {
  drive_update_state(DRIVE_BRAKE);
  motor_brake(motor_right);
  motor_brake(motor_left);
}

//--------------------------------------------
void drive_coast(void) {
  drive_update_state(DRIVE_COAST);
  motor_coast(motor_right);
  motor_coast(motor_left);
}

//--------------------------------------------
void drive_forward(int speed) {
  drive_update_state(DRIVE_FORWARD);
  motor_forward(motor_right, speed);
  motor_forward(motor_left, speed);
}

//--------------------------------------------
void drive_backward(int speed) {
  drive_update_state(DRIVE_BACKWARD);
  motor_reverse(motor_right, speed);
  motor_reverse(motor_left, speed);
}

//--------------------------------------------
void drive_pivot_ccw(int speed) {
  drive_update_state(DRIVE_PIVOT_CCW);
  motor_forward(motor_right, speed);
  motor_reverse(motor_left, speed);
}

//--------------------------------------------
void drive_pivot_cw(int speed) {
  drive_update_state(DRIVE_PIVOT_CW);
  motor_reverse(motor_right, speed);
  motor_forward(motor_left, speed);
}

//--------------------------------------------
void drive_steer_cw(int speed, int speed_steer) {
  drive_update_state(DRIVE_STEER_CW);
  #ifdef MOTOR_USE_PWM
    motor_reverse(motor_right, speed_steer);
    motor_forward(motor_left, speed);
  #else
    motor_coast(motorA);
    motor_forward(motorB);
  #endif
}

//--------------------------------------------
void drive_steer_ccw(int speed, int speed_steer) {
  drive_update_state(DRIVE_STEER_CCW);
  #ifdef MOTOR_USE_PWM
    motor_forward(motor_right, speed);
    motor_reverse(motor_left, speed_steer);
  #else
    motor_forward(PIN_MOTOR_AIN1, PIN_MOTOR_AIN2);
    motor_coast(PIN_MOTOR_BIN1, PIN_MOTOR_BIN2);
  #endif
}