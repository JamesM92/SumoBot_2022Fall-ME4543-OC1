/**
 * @file drive.h
 * @author M. Good
 * @brief DRIVE MOTOR
 * @version 0.1
 * @date 2022-06-20
 * 
 * @copyright Copyright (c) 2022
 */
#ifndef DRIVE_h
#define DRIVE_h

#include "motor.h"

// ** BEGIN SETTINGS **
#ifndef SETTINGS_h

  //#define DRIVE_REPORT_STATE_CHANGES     // comment out to disable state change reporting to console

  #ifdef MOTOR_USE_PWM
    #define DRIVE_SPEED_DEFAULT   255 //255 //
    #define DRIVE_SPEED_PIVOT     255 //
    #define DRIVE_SPEED_SEEK      255 //
    #define DRIVE_SPEED_STEER     125  // 70 set the default steering speed. PWM mode only.
  #endif

#endif

// ** END SETTINGS **

enum drive_states {
  DRIVE_UNDEFINED,
  DRIVE_BRAKE,
  DRIVE_COAST,
  DRIVE_FORWARD,
  DRIVE_BACKWARD,
  DRIVE_PIVOT_CCW,
  DRIVE_PIVOT_CW,
  DRIVE_STEER_CW,
  DRIVE_STEER_CCW,
};

void drive_setup(void);
bool drive_get_enabled(void);
void drive_set_enabled(bool state);

void drive_update_state(int state);
int drive_get_state(void);
void drive_report_state(int state);
bool drive_report_state_change(void);

// functions for both motors/channels
void drive_brake(void);
void drive_coast(void);
void drive_forward(int speed);
void drive_backward(int speed);
void drive_pivot_ccw(int speed);
void drive_pivot_cw(int speed);
void drive_steer_cw(int speed, int speed_steer);
void drive_steer_ccw(int speed, int speed_steer);

#endif