/**
 * @file distance_sensors.cpp
 * @author M. Good
 * @brief DISTANCE SENSORS (VL53L0X Laser Range Finder)
 * @version 0.1
 * @date 2022-06-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "distance_sensors.h"

VL53L0X ds_sensors[DS_COUNT];
uint16_t ds_values[DS_COUNT];

int ds_state_sensors = -1;
int ds_state_sensors_old = -1;
int ds_event_count = 0;

#ifdef DS_USE_TCA9548A

  // Select sensor from TCA9548A I2C Multiplexer
  void ds_select_sensor(int index) {
    if (index > 7) return;
    Wire.beginTransmission(I2C_MUX_ADDR);
    Wire.write(1 << index);
    Wire.endTransmission(); 
  }

  void ds_setup_sensor(int index) {
    ds_select_sensor(index);
    ds_sensors[index].setTimeout(DS_TIMEOUT);
    if (!ds_sensors[index].init()) {
      Serial.print(F("Failed to setup distance sensor "));
      Serial.println(index);
      while (1) {}
    }

    #ifdef DS_MODE_CONTINUOUS

      // can provide a desired inter-measurement period in ms (e.g. sensor.startContinuous(100)).
      ds_sensors[index].startContinuous();

    #else

      #if defined DS_MODE_LONG_RANGE
        // lower the return signal rate limit (default is 0.25 MCPS)
        sensor.setSignalRateLimit(0.1);
        // increase laser pulse periods (defaults are 14 and 10 PCLKs)
        sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
        sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
      #endif

      #ifdef DS_MODE_HIGH_SPEED
        ds_sensors[index].setMeasurementTimingBudget(DS_MODE_HIGH_SPEED);    // faster read at cost of accuracy
      #elif defined DS_MODE_HIGH_ACCURACY
        ds_sensors[index].setMeasurementTimingBudget(DS_MODE_HIGH_ACCURACY); // more accuracy at cost of read speed
      #endif
      
    #endif
  }

  void ds_setup_sensors(void) {
    for (uint8_t i = 0; i < DS_COUNT; i++){
      ds_setup_sensor(i);
    }
  }

#else

  void ds_setup_sensor(uint8_t address, int iopin) {
    // this function sets the address for a VL53L0X sensor
    #ifdef VERBOSE_DEBUG_INFO
      Serial.print(F("sensor_setup("));
      Serial.print(address);
      Serial.print(F(","));
      Serial.print(iopin);
      Serial.println(F(")"));
    #endif
    ds_sensors[address].setBus(&Wire);
    digitalWrite(iopin, HIGH);
    if(!ds_sensors[address].init(true)){
      Serial.print(F("ds_setup_sensor("));
      Serial.print(address);
      Serial.print(F(","));
      Serial.print(iopin);
      Serial.println(F(") failed."));
    }
    ds_sensors[address].setAddress(address + 1);
  }

  void ds_setup_sensors(void) {
    
    pinMode(PIN__XSHUT1, OUTPUT);
    pinMode(PIN__XSHUT2, OUTPUT);
    pinMode(PIN__XSHUT3, OUTPUT);
    pinMode(PIN__XSHUT4, OUTPUT);
    pinMode(PIN__XSHUT5, OUTPUT);
    pinMode(PIN__XSHUT6, OUTPUT);
    
    digitalWrite(PIN__XSHUT1, LOW);
    digitalWrite(PIN__XSHUT2, LOW);
    digitalWrite(PIN__XSHUT3, LOW);
    digitalWrite(PIN__XSHUT4, LOW);
    digitalWrite(PIN__XSHUT5, LOW);
    digitalWrite(PIN__XSHUT6, LOW);

    Wire.begin();

    // seting addresses
    ds_setup_sensor(0, PIN__XSHUT1);
    ds_setup_sensor(1, PIN__XSHUT2);
    ds_setup_sensor(2, PIN__XSHUT3);
    ds_setup_sensor(3, PIN__XSHUT4);
    ds_setup_sensor(4, PIN__XSHUT5);
    ds_setup_sensor(5, PIN__XSHUT6);

    // start sensors reading continuously
    ds_sensors[0].startContinuous();
    ds_sensors[1].startContinuous();
    ds_sensors[2].startContinuous();
    ds_sensors[3].startContinuous();
    ds_sensors[4].startContinuous();
    ds_sensors[5].startContinuous();
  }

#endif

uint16_t ds_read_sensor(int index) {

  uint16_t sensor_value = 0;

  //ds_select_sensor(index);

  #ifdef DS_MODE_CONTINUOUS
    sensor_value = ds_sensors[index].readRangeContinuousMillimeters();
  #else
    sensor_value = ds_sensors[index].readRangeSingleMillimeters();
  #endif

  if (ds_sensors[index].timeoutOccurred()) { 
    Serial.print(F(" TIMEOUT")); 
  }

  return sensor_value;

}

void ds_update_sensor(int index) {

  ds_values[index] = ds_read_sensor(index);
  
  // limit the distance the sensor can report
  if(ds_values[index] > DS_DIST_FAR) {
    ds_values[index] = DS_DIST_FAR;
  }

  // update the sensor array's state
  bitWrite(ds_state_sensors, index, ds_values[index] < DS_DIST_FAR);

}

void ds_update_sensors(void) {
  ds_state_sensors = 0;
  for(int i = 0; i < DS_COUNT; i++){
    ds_update_sensor(i);
  }

  #ifdef DS_REPORT_STATE_CHANGES
    if(ds_report_state_change_name()) Serial.println();
  #endif

}

int ds_get_state_sensor(int index) {
  return ds_values[index] < DS_DIST_FAR;
}

int ds_get_state_sensors(void) {
  
  ds_update_sensors();
  
  int array_state = 0;
  for (int i = 0; i < DS_COUNT; i++) {
    // filter out sensors reading DS_DIST_FAR and above.
    if(ds_get_state_sensor(i)) {
      array_state += 1 << i;
    }
  }
  return array_state;
}

void ds_report_value(int sensor_number) {
  Serial.print(ds_values[sensor_number]);
}

void ds_report_values(void) {
  for (uint8_t i = 0; i < DS_COUNT; i++){
    ds_report_value(i);
    Serial.print(F(","));
  }
}

void ds_report_state_name_sensors(int state) {
  Serial.print(F(" DS_"));
  switch(state) {
    case DS_STATE_NONE: Serial.print(F("NONE")); break;
    case DS_STATE_FC  : Serial.print(F("FC")); break;
    case DS_STATE_FL  : Serial.print(F("FL")); break;
    case DS_STATE_FCL : Serial.print(F("FCFL")); break;
    case DS_STATE_FR  : Serial.print(F("FR")); break;
    case DS_STATE_FCR : Serial.print(F("FCFR")); break;
    case DS_STATE_FCLR: Serial.print(F("FCFLFR")); break;
    case DS_STATE_SL  : Serial.print(F("SL")); break;
    case DS_STATE_SR  : Serial.print(F("SR")); break;
    case DS_STATE_BC  : Serial.print(F("RC")); break;
    default           : Serial.print(F("INVALID")); break;
  }
}

bool ds_report_state_change_name(void) {
  if(ds_state_sensors != ds_state_sensors_old) {
    ds_state_sensors_old = ds_state_sensors;
    ds_report_state_name_sensors(ds_state_sensors);
    return true;
  }
  return false;
}