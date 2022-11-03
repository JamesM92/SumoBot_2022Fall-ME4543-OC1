/**
 * @file line_sensors.h
 * @author M. Good
 * @brief LINE SENSORS (QRE1113 Photointerrupter Reflective Phototransistor)
 * @version 0.1
 * @date 2022-06-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef LINE_SENSORS_h
#define LINE_SENSORS_h

// ** BEGIN SETTINGS **
#ifndef SETTINGS_h

  //#define LS_REPORT_STATE_CHANGES     // comment out to disable state change reporting to console

  #define LS_ENABLED                  // comment out to disable line sensors
  #define USE_LINE_THRESHOLD_WHITE    // comment out to use black as the threshold

  #define LS_THRESHOLD_WHITE    200   // lesser values are considered white.
  #define LS_THRESHOLD_BLACK    800   // greater values are considered black or surface out of range.

#endif

// ** END SETTINGS **

#define LINE_SENSOR_ANALOG_QRE1113  // comment out to use digital version of QRE1113

#define LS_COUNT  4

// define pins
#define PIN_LS_FR   A0  // LS_FR - LINE SENSOR FRONT RIGHT
#define PIN_LS_FL   A1  // LS_FL - LINE SENSOR FRONT LEFT
#define PIN_LS_BL   A6  // LS_BR - LINE SENSOR BACK LEFT
#define PIN_LS_BR   A7  // LS_FR - LINE SENSOR BACK RIGHT

// define sensor index/id
#define LS_FL   0
#define LS_FR   1
#define LS_BL   2
#define LS_BR   3

void ls_setup_sensors(void);

int ls_read_sensor(int index);
void ls_update_sensor(int index);
void ls_update_sensors(void);

int ls_get_state_sensor(int index);
int ls_get_state_sensors(void);

void ls_report_value(int index);
void ls_report_values(void);
void ls_report_state_name_sensors(int state);
bool ls_report_state_change_name(void);

int ls_get_state_front(void);
int ls_get_state_back(void);

#endif