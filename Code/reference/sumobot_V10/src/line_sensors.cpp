/**
 * @file line_sensors.cpp
 * @author M. Good
 * @brief LINE SENSORS (QRE1113 Photointerrupter Reflective Phototransistor)
 * @version 0.1
 * @date 2022-06-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <Arduino.h>
#include "line_sensors.h"

int ls_sensors[LS_COUNT] =  {PIN_LS_FL, PIN_LS_FR, PIN_LS_BL, PIN_LS_BR};
int ls_values[LS_COUNT];

int ls_state_sensors = -1;
int ls_state_sensors_old = -1;

void ls_setup_sensors(void) {
  #ifdef LS_REPORT_STATE_CHANGES
    Serial.println("ls_setup_sensors");
  #endif
  //ls_update_sensors();
}

int ls_read_sensor(int index) {

  int pin = ls_sensors[index];

  #ifdef LINE_SENSOR_ANALOG_QRE1113
    return analogRead(pin);
  #else
    //Returns value from the QRE1113 
    //Lower numbers mean more refleacive
    //More than 3000 means nothing was reflected.
    pinMode(pin, OUTPUT );
    digitalWrite(pin, HIGH );  
    delayMicroseconds(10);
    pinMode(pin, INPUT );

    long time = micros();

    //time how long the input is HIGH, but quit after 3ms as nothing happens after that
    while (digitalRead(pin) == HIGH && micros() - time < 3000); 
    int diff = micros() - time;

    return diff;
  #endif
}

void ls_update_sensor(int index) {
  ls_values[index] = ls_read_sensor(index);
}

void ls_update_sensors(void) {
  // store values in the array
  ls_values[LS_FL] = ls_read_sensor(LS_FL);
  ls_values[LS_FR] = ls_read_sensor(LS_FR);
  ls_values[LS_BL] = ls_read_sensor(LS_BL);
  ls_values[LS_BR] = ls_read_sensor(LS_BR);
  
  // compute the sensor group's state
  ls_state_sensors = 0;
  ls_state_sensors += ls_get_state_sensor(LS_FL) << LS_FL;
  ls_state_sensors += ls_get_state_sensor(LS_FR) << LS_FR;
  ls_state_sensors += ls_get_state_sensor(LS_BL) << LS_BL;
  ls_state_sensors += ls_get_state_sensor(LS_BR) << LS_BR;

  #ifdef LS_REPORT_STATE_CHANGES
    if(ls_report_state_change_name()) Serial.println();
  #endif

}

int ls_get_state_sensor(int index) {
  #ifdef LS_ENABLED
    #ifdef USE_LINE_THRESHOLD_WHITE
      return ls_values[index] <= LS_THRESHOLD_WHITE;
    #else
      return ls_values[index] >= LS_THRESHOLD_BLACK;
    #endif
  #else
    return 0;
  #endif
}

int ls_get_state_sensors(void) {
  return ls_state_sensors;
}

void ls_report_value(int sensor_number) {
  Serial.print(ls_values[sensor_number]);
}

void ls_report_values(void) {
  ls_update_sensors();
  ls_report_value(LS_FL);
  Serial.print(F(","));
  ls_report_value(LS_FR);
  Serial.print(F(","));
  ls_report_value(LS_BL);
  Serial.print(F(","));
  ls_report_value(LS_BR);
  Serial.println();
}

void ls_report_state_name_sensors(int state) {
  Serial.print(F(" LS_"));
  switch(state) {
    case 0  : Serial.print(F("NONE")); break;
    case 1  : Serial.print(F("FL")); break;
    case 2  : Serial.print(F("FR")); break;
    case 3  : Serial.print(F("FRFL")); break;
    case 4  : Serial.print(F("BL")); break;
    case 5  : Serial.print(F("BLFL")); break;
    case 6  : Serial.print(F("BLFR")); break;
    case 7  : Serial.print(F("BLFRFL")); break;
    case 8  : Serial.print(F("BR")); break;
    case 9  : Serial.print(F("BRFL")); break;
    case 10 : Serial.print(F("BRFR")); break;
    case 11 : Serial.print(F("BRFRFL")); break;
    case 12 : Serial.print(F("BRBL")); break;
    case 13 : Serial.print(F("BRBLFL")); break;
    case 14 : Serial.print(F("BRBLFR")); break;
    case 15 : Serial.print(F("BRBLFRFL")); break;
    default : Serial.print(F("UNKNOWN")); break;
  }
}

bool ls_report_state_change_name(void) {
  if(ls_state_sensors != ls_state_sensors_old) {
    ls_state_sensors_old = ls_state_sensors;
    ls_report_state_name_sensors(ls_state_sensors);
    return true;
  } else {
    return false;
  }
}


int ls_get_state_front(void) {
  #if 0
  if((ls_get_state_sensor(LS_FL) || ls_get_state_sensor(LS_FR)) > 0) {
    #if 1
      Serial.print(F("LS_F("));
      Serial.print(ls_values[LS_FL]);
      Serial.print(F(", "));
      Serial.print(ls_values[LS_FR]);
      Serial.print(F(")"));
    #endif
    return 1;
  }else{
    return 0;
  }
  #else
    #ifdef LS_ENABLED
      #ifdef USE_LINE_THRESHOLD_WHITE
      
        if(analogRead(PIN_LS_FL) <= LS_THRESHOLD_WHITE) {
          #ifdef LS_REPORT_STATE_CHANGES
            Serial.println("LS_FL");
          #endif
          return 1;
        } else if(analogRead(PIN_LS_FR) <= LS_THRESHOLD_WHITE) {
          #ifdef LS_REPORT_STATE_CHANGES
            Serial.println("LS_FR");
          #endif
          return 1;
        } else {
          return 0;
        }
      #else
        if(analogRead(PIN_LS_FL) <= LS_THRESHOLD_BLACK) {
          #ifdef LS_REPORT_STATE_CHANGES
            Serial.println("LS_FL");
          #endif
          return 1;
        }else if(analogRead(PIN_LS_FR) <= LS_THRESHOLD_BLACK) {
          #ifdef LS_REPORT_STATE_CHANGES
            Serial.println("LS_FR");
          #endif
          return 1;
        } else {
          return 0;
        }
      #endif
    #else
      return 0;
    #endif
  #endif
}

int ls_get_state_back(void) {
  #if 0
  if((ls_get_state_sensor(LS_BL) || ls_get_state_sensor(LS_BR)) > 0) {
    #if 1
      Serial.print(F("LS_B("));
      Serial.print(ls_values[LS_BL]);
      Serial.print(F(", "));
      Serial.print(ls_values[LS_BR]);
      Serial.print(F(")"));
    #endif
    return 1;
  }else{
    return 0;
  }
  #else

    #ifdef LS_ENABLED
      #ifdef USE_LINE_THRESHOLD_WHITE
        if(analogRead(PIN_LS_BL) <= LS_THRESHOLD_WHITE) {
          #ifdef LS_REPORT_STATE_CHANGES
            Serial.println("LS_BL");
          #endif
          return 1;
        } else if(analogRead(PIN_LS_BR) <= LS_THRESHOLD_WHITE) {
          #ifdef LS_REPORT_STATE_CHANGES
            Serial.println("LS_BR");
          #endif
          return 1;
        } else {
          return 0;
        }
      #else
        if(analogRead(PIN_LS_BL) <= LS_THRESHOLD_BLACK) {
          #ifdef LS_REPORT_STATE_CHANGES
            Serial.println("LS_BL");
          #endif
          return 1;
        } else if(analogRead(PIN_LS_BR) <= LS_THRESHOLD_BLACK) {
          #ifdef LS_REPORT_STATE_CHANGES
            Serial.println("LS_BR");
          #endif
          return 1;
        } else {
          return 0;
        }
      #endif
    #else
      return 0;
    #endif
  #endif
}