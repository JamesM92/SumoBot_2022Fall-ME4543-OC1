/**
 * @file settings.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef SETTINGS_h
#define SETTINGS_h

// -----------------------------------------------------------
// line_sensors.h --------------------------------------------
// -----------------------------------------------------------

//#define LS_REPORT_STATE_CHANGES     // comment out to disable state change reporting to console

#define LS_ENABLED                  // comment out to disable line sensors
#define USE_LINE_THRESHOLD_WHITE    // comment out to use black as the threshold

#define LS_THRESHOLD_WHITE    (200)   // lesser values are considered white.
#define LS_THRESHOLD_BLACK    (800)   // greater values are considered black or surface out of range.

// ---------------------------------------------------------------
// distance_sensors.h --------------------------------------------
// ---------------------------------------------------------------

//#define DS_REPORT_STATE_CHANGES   // comment out to disable state change reporting to console

//#define DS_USE_TCA9548A           // uncomment if using the TCA9548A I2C mux breakout board

#define DS_COUNT          6       // when using a TCA9548A the max sensor count is 8

#define DS_DIST_FAR       (8*25)    // detect limit (should be diameter of arena = 770 mm (30.315 in))
#define DS_DIST_MID       75
#define DS_DIST_CLOSE     50 

#define DS_TIMEOUT        500

// uncomment the following line for continuous mode (possible neighboring sensor interference)
// Continuous mode: 30 ms to read 6 sensors. 
// I encountered interference among the front three sensors when the design had the beams cross.
// The symptom of interference was eratic values among the sensors.
// No interference wWhen using single shot mode: 139 ms to read 6 sensors (timing budget 20000 (20 ms)).
#define DS_MODE_CONTINUOUS // default is single shot mode. 

#ifndef DS_MODE_CONTINUOUS

  //#define DS_MODE_LONG_RANGE
  
  // optionally uncomment one of the following two modes. default timing budget 33000 (33 ms).
  //#define DS_MODE_HIGH_SPEED    20000  // high speed timing budget 20000 (20 ms)
  //#define DS_MODE_HIGH_ACCURACY 200000 // high accuracy timing budget 200000 (200 ms)

#endif

// -----------------------------------------------------
// motor.h --------------------------------------------
// -----------------------------------------------------

//#define MOTOR_REPORT_STATE_CHANGES     // comment out to disable state change reporting to console

#define MOTOR_USE_PWM               // uncomment to use motors in PWM mode
//#define MOTOR_SLOW_DECAY          // uncomment to use motors in slow decay mode

#define MOTOR_PWM_MIN         0
#define MOTOR_PWM_MAX         255

// -----------------------------------------------------
// drive.h --------------------------------------------
// -----------------------------------------------------

//#define DRIVE_REPORT_STATE_CHANGES     // comment out to disable state change reporting to console

#ifdef MOTOR_USE_PWM
  #define DRIVE_SPEED_DEFAULT   255 //255 //
  #define DRIVE_SPEED_PIVOT     255 //
  #define DRIVE_SPEED_SEEK      255 //
  #define DRIVE_SPEED_STEER     70  // set the default steering speed. PWM mode only.
#endif

// ----------------------------------------------------
// robot.h --------------------------------------------
// ----------------------------------------------------

//#define ROBOT_REPORT_STATE_CHANGES  // comment out to disable state change reporting to console

#define MSM_STARTUP_DELAY         1000

// Uncomment one of the following power upply options.
//#define POWER_USB_PC
#define POWER_USB_WALLWART
//#define POWER_LIPO

#endif