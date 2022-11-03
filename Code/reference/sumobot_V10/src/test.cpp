/**
 * @file test.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "test.h"
#include "drive.h"
#include "line_sensors.h"
#include "distance_sensors.h"
#include "mouse.h"


// How to measure the supply voltage of Arduino without using an analog pin
// https://arduino.stackexchange.com/questions/50769/how-to-measure-the-supply-voltage-of-arduino-without-using-an-analog-pin
long readVcc(void) {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  // 1125300 is 1100 (1.1v as mv) times 1023 (ADC max value)
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}

void test_ls(void) {
  
  ls_update_sensors();

  #if 1
    ls_report_values();
  #elif 0
    // test the front and back line sensors
    if(ls_get_state_front()){
      Serial.println();
    }
    if(ls_get_state_back()){
      Serial.println();
    }
  #elif 0
    ls_report_state_change_name();
  #else
    // report the state of the sensor array
    if(ls_get_sensors_state() > 0) {
      Serial.println(ls_get_sensors_state());
    }
  #endif
}

void test_ds(void) {

  ds_update_sensors();

  #if 0
    ds_report_state_change_name();
  #elif 1
    // print csv of distance sensor values. use with SerialPlot.
    ds_report_values();
    Serial.println();
  #else
    // get the single state value describing the sensors array and print to the console
    int state = get_state_distance_sensors();
    if(state > 0) {
      Serial.println(state);
    }
  #endif
}

void test_motors(void) {

    #if 1
      //drive_forward(DRIVE_SPEED_DEFAULT);
      drive_backward(DRIVE_SPEED_DEFAULT);

    #elif 0
      drive_forward(DRIVE_SPEED_DEFAULT);
      delay(1000);
      drive_brake();
      delay(1000);

      drive_backward(DRIVE_SPEED_DEFAULT);
      delay(1000);
      drive_brake();
      delay(1000);

      drive_steer_left(DRIVE_SPEED_DEFAULT, DRIVE_SPEED_STEER);
      delay(1000);
      drive_brake();
      delay(1000);

      drive_backward(DRIVE_SPEED_DEFAULT);
      delay(1000);
      drive_brake();
      delay(1000);

      drive_steer_right(DRIVE_SPEED_DEFAULT, DRIVE_SPEED_STEER);
      delay(1000);
      drive_brake();
      delay(1000);

      drive_backward(DRIVE_SPEED_DEFAULT);
      delay(1000);
      drive_brake();
      delay(1000);

      drive_rotate_left(DRIVE_SPEED_DEFAULT);
      delay(1000);
      drive_brake();
      delay(1000);

      drive_rotate_right(DRIVE_SPEED_DEFAULT);
      delay(1000);
      drive_brake();
      delay(1000);
      //delay(1000);
      //drive_brake();

    #elif 0
      drive_forward(DRIVE_SPEED_DEFAULT);
      delay(1000);
      drive_brake();

      for(int i = 0; i < 5; i++){
        drive_rotate_left(DRIVE_SPEED_DEFAULT);
        delay(200);
        drive_brake();
        drive_rotate_right(DRIVE_SPEED_DEFAULT);
        delay(200);
        drive_brake();
      }

      drive_backward(DRIVE_SPEED_DEFAULT);
      delay(1000);
      drive_brake();

      drive_steer_left(DRIVE_SPEED_DEFAULT, 20);
      delay(1000);
      drive_brake();
      drive_steer_right(DRIVE_SPEED_DEFAULT, 20);
      delay(1000);
      drive_brake();

      delay(100);

      drive_backward(DRIVE_SPEED_DEFAULT);
      delay(1500);
      drive_brake();

      delay(1000);
  #endif
}

void test_mouse(void) {
  while(1) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    mouse_report();
    delay(20);
  }
}