/**
 * @file motor.cpp
 * @author M. Good
 * @brief MOTOR DRIVER (DRV8833 Dual H-Bridge Motor Driver)
 * @version 0.1
 * @date 2022-07-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <Arduino.h>
#include "motor.h"

void motor_update_state(motor m, int mode, int state, int speed) {
  m.mode = mode;
  m.state = state;
  m.speed = speed;

  #ifdef MOTOR_REPORT_STATE_CHANGES
    if(motor_report_state_change(m)) Serial.println();
  #endif

}

bool motor_report_state_change(motor m) {
  // report state change.
  if(m.state != m.state_old) {
    m.state_old = m.state;
    motor_report_state(m);
    return true;
  } else {
    return false;
  }
}

void motor_report_state(motor m) {
  Serial.print(F(" MOTOR_"));
  Serial.print(m.id);
  Serial.print(F(" "));

  switch(m.mode) {
    case MOTOR_CONSTANT: Serial.print(F("CONSTANT")); break;
    case MOTOR_FAST_DECAY: Serial.print(F("FAST_DECAY")); break;
    case MOTOR_SLOW_DECAY: Serial.print(F("SLOW_DECAY")); break;
  }
  Serial.print(F(" "));

  switch(m.state) {
    case MOTOR_COAST: Serial.print(F("COAST")); break;
    case MOTOR_REVERSE: Serial.print(F("REVERSE")); break;
    case MOTOR_FORWARD: Serial.print(F("FORWARD")); break;
    case MOTOR_BRAKE: Serial.print(F("BRAKE")); break;
  }
  Serial.print(F(" "));

  Serial.print(m.speed);
}


//--------------------------------------------
void motor_brake(motor m) {
  // xIN1 = HIGH, xIN2 = HIGH ---> xOUT1 = LOW, xOUT2 = LOW
  digitalWrite(m.pin_in1, HIGH); 
  digitalWrite(m.pin_in2, HIGH);
  motor_update_state(m, MOTOR_CONSTANT, MOTOR_BRAKE, MOTOR_PWM_MIN);
}

//--------------------------------------------
void motor_coast(motor m) {
  // xIN1 = LOW, xIN2 = LOW ---> xOUT1 = Z, xOUT2 = Z
  digitalWrite(m.pin_in1, LOW); 
  digitalWrite(m.pin_in2, LOW);
  motor_update_state(m, MOTOR_CONSTANT, MOTOR_COAST, MOTOR_PWM_MIN);
}

//--------------------------------------------
void motor_forward(motor m) {
  // xIN1 = LOW, xIN2 = HIGH ---> xOUT1 = LOW, xOUT2 = HIGH
  digitalWrite(m.pin_in1, HIGH); 
  digitalWrite(m.pin_in2, LOW);
  motor_update_state(m, MOTOR_CONSTANT, MOTOR_FORWARD, MOTOR_PWM_MAX);
}

//--------------------------------------------
void motor_forward(motor m, int speed) {
  #ifdef MOTOR_USE_PWM
    #ifdef MOTOR_SLOW_DECAY
      // Forward PWM, slow decay
      digitalWrite(m.pin_in1, HIGH);
      analogWrite(m.pin_in2, speed);
      motor_update_state(m, MOTOR_SLOW_DECAY, MOTOR_FORWARD, speed);
    #else
      // Forward PWM, fast decay
      analogWrite(m.pin_in1, speed);
      digitalWrite(m.pin_in2, LOW);
      motor_update_state(m, MOTOR_FAST_DECAY, MOTOR_FORWARD, speed);
    #endif
  #else
    motor_forward(m);
  #endif
}

//--------------------------------------------
void motor_reverse(motor m) {
  // xIN1 = HIGH, xIN2 = LOW ---> xOUT1 = HIGH, xOUT2 = LOW
  digitalWrite(m.pin_in1, LOW); 
  digitalWrite(m.pin_in2, HIGH);
  motor_update_state(m, MOTOR_CONSTANT, MOTOR_REVERSE, MOTOR_PWM_MAX);
}

//--------------------------------------------
void motor_reverse(motor m, int speed) {
  #ifdef MOTOR_USE_PWM
    #ifdef MOTOR_SLOW_DECAY
      // Reverse PWM, slow decay
      analogWrite(m.pin_in1, speed);
      digitalWrite(m.pin_in2, HIGH);
      motor_update_state(m, MOTOR_SLOW_DECAY, MOTOR_REVERSE, speed);
    #else
      // Reverse PWM, fast decay
      digitalWrite(m.pin_in1, LOW);
      analogWrite(m.pin_in2, speed);
      motor_update_state(m, MOTOR_FAST_DECAY, MOTOR_REVERSE, speed);
    #endif
  #else
    motor_reverse(m);
  #endif

}