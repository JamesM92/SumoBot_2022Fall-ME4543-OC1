/**
 * @file robot.h
 * @author M. GOOD
 * @brief 
 * @version 0.1
 * @date 2022-06-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef ROBOT_h
#define ROBOT_h

// ** BEGIN SETTINGS **
#ifndef SETTINGS_h

  #define ROBOT_REPORT_STATE_CHANGES  // comment out to disable state change reporting to console

  #define MSM_STARTUP_DELAY         1000

  // Uncomment one of the following power supply options.
  //#define POWER_USB_PC
  //#define POWER_USB_WALLWART
  //#define POWER_LIPO
  #define POWER_8AA

#endif
// ** END SETTINGS **

#ifdef POWER_LIPO
  #define MSM_PANIC_DRIVE_DURATION  200
  #define MSM_PANIC_PIVOT_DURATION  400
  #define MSM_SEEK_DURATION         50
#endif

#ifdef POWER_USB_WALLWART
  #define MSM_PANIC_DRIVE_DURATION  200
  #define MSM_PANIC_PIVOT_DURATION  850
  #define MSM_SEEK_DURATION         400
#endif

#ifdef POWER_USB_PC
  #define MSM_PANIC_DRIVE_DURATION  300
  #define MSM_PANIC_PIVOT_DURATION  1000
  #define MSM_SEEK_DURATION         1000
#endif

#ifdef POWER_8AA
  #define MSM_PANIC_DRIVE_DURATION  400
  #define MSM_PANIC_PIVOT_DURATION  800
  #define MSM_SEEK_DURATION         100
#endif

enum robot_states {
  UNDEFINED,
  STARTUP,
  STARTUP_DELAY,
  BEGIN,
  BEGIN_ADVANCE,
  BEGIN_PIVOT,
  SEEK_AND_DESTROY,
  SEEK_LEFT,
  SEEK_RIGHT,
  PANIC_FRONT,
  PANIC_FRONT_STEP_2,
  PANIC_FRONT_STEP_3,
  PANIC_REAR,
  PANIC_REAR_STEP_2,
  PANIC_REAR_STEP_3,
};

void robot_update_sm(void);

//void robot_report_state_name(int state);
bool robot_report_state_change_name(void);

#endif
