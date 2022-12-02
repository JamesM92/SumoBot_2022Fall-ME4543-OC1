/**
 * @file distance_sensors.h
 * @author M. Good
 * @brief DISTANCE SENSORS (VL53L0X Laser Range Finder)
 * @version 0.1
 * @date 2022-06-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef DISTANCE_SENSORS_h
#define DISTANCE_SENSORS_h

#include <Arduino.h>
#include "VL53L0X.h"

// ** BEGIN SETTINGS **
#ifndef SETTINGS_h

  #define DS_REPORT_STATE_CHANGES   // comment out to disable state change reporting to console

  //#define DS_USE_TCA9548A           // uncomment if using the TCA9548A I2C mux breakout board

  #define DS_COUNT          6       // when using a TCA9548A the max sensor count is 8

  #define DS_DIST_FAR      500 //(8*25)    // detect limit (should be diameter of arena = 770 mm (30.315 in))
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
#endif
// ** END SETTINGS **

// sensor position in sensor arrays
#define DS_FC 0 // DS_FC front center
#define DS_FL 1 // DS_FL front left
#define DS_FR 2 // DS_FR front right
#define DS_SL 3 // DS_SL side left
#define DS_SR 4 // DS_SR side right
#define DS_BC 5 // DS_BC back center

// valid sensor combinations with one oponent.
enum ds_states {
  DS_STATE_NONE = 0,
  DS_STATE_FC   = 1,
  DS_STATE_FL   = 2,
  DS_STATE_FCL  = 3,
  DS_STATE_FR   = 4,
  DS_STATE_FCR  = 5,
  DS_STATE_FCLR = 7,
  DS_STATE_SL   = 8,
  DS_STATE_SR   = 16,
  DS_STATE_BC   = 32,
};

int ds_get_state_sensor(int index);
int ds_get_state_sensors(void);

void ds_update_sensor(int index);
void ds_update_sensors(void);

void ds_report_state_name_sensors(int state);
bool ds_report_state_change_name(void);
void ds_report_value(int index);
void ds_report_values(void);

#ifdef DS_USE_TCA9548A

  #define I2C_MUX_ADDR          0x70  // TCA9548A default address. A0, A1, and A2 are pulled LOW.

  void ds_select_sensor(int index);
  void ds_setup_sensor(int index);
  void ds_setup_sensors(void);

#else

  // define pins
  #define PIN__XSHUT1   12 //7   // DS_FC front center
  #define PIN__XSHUT2    8 //8   // DS_FL front left
  #define PIN__XSHUT3    4 //2   // DS_FR front right
  #define PIN__XSHUT4   13 //12  // DS_SL side left
  #define PIN__XSHUT5    2 //A3  // DS_SR side right
  #define PIN__XSHUT6    7 //A2  // DS_BC back center

  void ds_setup_sensor(uint8_t address, int iopin);
  void ds_setup_sensors(void);

#endif

#endif
