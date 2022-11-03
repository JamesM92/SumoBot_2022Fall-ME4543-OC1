/**
 * @file motor.h
 * @author M. Good
 * @brief MOTOR DRIVER (DRV8833 Dual H-Bridge Motor Driver)
 * @version 0.1
 * @date 2022-07-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MOTOR_h
#define MOTOR_h

// ** BEGIN SETTINGS **
#ifndef SETTINGS_h

  //#define MOTOR_REPORT_STATE_CHANGES     // comment out to disable state change reporting to console

  #define MOTOR_USE_PWM               // uncomment to use motors in PWM mode
  //#define MOTOR_SLOW_DECAY          // uncomment to use motors in slow decay mode

  #define MOTOR_PWM_MIN         0
  #define MOTOR_PWM_MAX         255

#endif
// ** END SETTINGS **

/*
* DRV8833 PWM frequency can be 1 Hz to 50 kHz.
* Nano pins D3, D9, D10, and D11 default PWM frequency is 490.20 Hz
* Nano pins D5 & D6 default PWM frequency is 976.56 Hz
*/
#define PIN_MOTOR_STBY    4
#define PIN_MOTOR_AIN1    3   // AO1 - RIGHT MOTOR  +
#define PIN_MOTOR_AIN2    9   // AO2 - RIGHT MOTOR  -
#define PIN_MOTOR_BIN1    10  // BO1 - LEFT MOTOR   +
#define PIN_MOTOR_BIN2    11  // BO2 - LEFT MOTOR   -

enum motor_mode{
  MOTOR_CONSTANT,
  MOTOR_FAST_DECAY,
  MOTOR_SLOW_DECAY,
};

enum motor_state{
  MOTOR_COAST,        // coast/fast decay
  MOTOR_REVERSE,      // reverse
  MOTOR_FORWARD,      // forward
  MOTOR_BRAKE,        // brake/slow decay
};

typedef struct {
  char id;
  int pin_in1;
  int pin_in2;
  int mode;
  int speed;
  int state;
  int state_old;
} motor;

void motor_update_state(motor m, int mode, int state, int speed);
void motor_report_state(motor m);
bool motor_report_state_change(motor m);

void motor_brake(motor m);
void motor_coast(motor m);
void motor_forward(motor m);
void motor_forward(motor m, int speed);
void motor_reverse(motor m);
void motor_reverse(motor m, int speed);

#endif