#include <Arduino.h>
#include <Wire.h>
//#include "settings.h"
#include "drive.h"
#include "line_sensors.h"
#include "distance_sensors.h"
#include "robot.h"

void setup() {
  Serial.begin (115200);
  Serial.println("main.setup()");
  //pinMode(LED_BUILTIN, OUTPUT);

  Wire.begin();
  ls_setup_sensors();
  ds_setup_sensors();
  drive_setup();
}

void loop() {

  //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  #if 1
    robot_update_sm();
  #else
    #include "test.h"
    #if 0
      test_ls();
    #endif
    #if 1
      test_ds();
    #endif
    #if 1
      test_motors();
    #endif
    #if 0
      Serial.print(readVcc());
      Serial.println(" mV");
      delay(1000);
    #endif
  #endif
}
