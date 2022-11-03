 /**
 * @file robot.cpp
 * @author M. GOOD
 * @brief 
 * @version 0.1
 * @date 2022-06-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <Arduino.h>
#include "robot.h"
#include "drive.h"
#include "line_sensors.h"
#include "distance_sensors.h"

#define VERBOSE_DEBUG_INFO

int robot_state = STARTUP;
int robot_state_old = UNDEFINED;

void robot_update_sm(void) {

  static unsigned long timer = 0;

  #ifdef ROBOT_REPORT_STATE_CHANGES
    if(robot_report_state_change_name()) Serial.println();
  #endif

  if(robot_state == STARTUP) {
    // at startup turn motors off and start timer
    drive_brake();
    timer = millis();
    robot_state = STARTUP_DELAY;  

  } else if (robot_state == STARTUP_DELAY) {
    // after a number of seconds advance state machine
    if((millis() - timer) >= MSM_STARTUP_DELAY) {
      robot_state = BEGIN;
      drive_forward(DRIVE_SPEED_DEFAULT);
    }
  
  } else if (robot_state == BEGIN) {
    #if 0
      drive_forward(DRIVE_SPEED_DEFAULT);
    #elif 1
      // continuous movement
      drive_forward(DRIVE_SPEED_DEFAULT);
      robot_state = SEEK_AND_DESTROY; 
    #elif 0
      // advance, pivot, seek and destroy.
      timer = millis();
      robot_state = BEGIN_ADVANCE;
    #else
      drive_brake();
      robot_state = SEEK_AND_DESTROY; 
    #endif

  } else if (robot_state == BEGIN_ADVANCE) {
    drive_forward(DRIVE_SPEED_DEFAULT);
    if((millis() - timer) >= MSM_PANIC_DRIVE_DURATION) {
      timer = millis();
      robot_state = BEGIN_PIVOT;
    }
  } else if (robot_state == BEGIN_PIVOT) {
    drive_pivot_ccw(255);
    if((millis() - timer) >= MSM_PANIC_PIVOT_DURATION / 2) {
      robot_state = SEEK_AND_DESTROY;
    }


  } else if (robot_state == PANIC_FRONT) { 
    // line sensor triggered... STOP! back up!
    drive_backward(255);
    timer = millis();
    robot_state = PANIC_FRONT_STEP_2;

  } else if (robot_state == PANIC_FRONT_STEP_2) {
    // after 1 second rotate and start timer
    if((millis() - timer) >= MSM_PANIC_DRIVE_DURATION) {
      drive_pivot_ccw(255);
      timer = millis();
      robot_state = PANIC_FRONT_STEP_3;
    }

  } else if (robot_state == PANIC_FRONT_STEP_3) {
    if((millis() - timer) >= MSM_PANIC_PIVOT_DURATION) {
      robot_state = BEGIN;
  }

  } else if (robot_state == PANIC_REAR) { 
    // line sensor triggered... STOP! GO FORWARD!
    drive_forward(255);
    timer = millis();
    robot_state = PANIC_REAR_STEP_2;

  } else if (robot_state == PANIC_REAR_STEP_2) {
    // after 1 second rotate and start timer
    if((millis() - timer) >= MSM_PANIC_DRIVE_DURATION) {
      drive_pivot_cw(255);
      timer = millis();
      robot_state = PANIC_REAR_STEP_3;
    }

  } else if (robot_state == PANIC_REAR_STEP_3) {
    if((millis() - timer) >= MSM_PANIC_PIVOT_DURATION) {
      robot_state = BEGIN;
    }




  } else if (robot_state == SEEK_AND_DESTROY) {

    if(ls_get_state_front() == 1) {
      drive_brake();
      robot_state = PANIC_FRONT;
    }else if(ls_get_state_back() == 1) {
      drive_brake();
      robot_state = PANIC_REAR;
    } else {
      // change state based on distance sensors
      switch(ds_get_state_sensors()) {

        case DS_STATE_FC: {
          drive_forward(DRIVE_SPEED_DEFAULT);
          break;
        }
        case DS_STATE_FL: {
          drive_pivot_ccw(DRIVE_SPEED_PIVOT);
          break;
        }
        case DS_STATE_FCL: {
          drive_steer_ccw(DRIVE_SPEED_DEFAULT, DRIVE_SPEED_STEER);
          break;
        }
        case DS_STATE_FR: {
          drive_pivot_cw(DRIVE_SPEED_PIVOT);
          break;
        }
        case DS_STATE_FCR: {
          drive_steer_cw(DRIVE_SPEED_DEFAULT, DRIVE_SPEED_STEER);
          break;
        }
        case DS_STATE_FCLR: {
          drive_forward(DRIVE_SPEED_DEFAULT);
          break;
        }

        case DS_STATE_SL: {
          drive_pivot_ccw(DRIVE_SPEED_SEEK);
          timer = millis();
          robot_state = SEEK_LEFT;
          break;
        }
        case DS_STATE_SR: {
          drive_pivot_cw(DRIVE_SPEED_SEEK);
          timer = millis();
          robot_state = SEEK_RIGHT;
          break;
        }
        case DS_STATE_BC: {
          drive_pivot_ccw(DRIVE_SPEED_SEEK);
          timer = millis();
          robot_state = SEEK_LEFT;
          break;
        }
        default: {
          //drive_brake();
          robot_state = BEGIN;
          break;
        }
      }
    }



    
  } else if(robot_state == SEEK_LEFT) {
        if((ds_get_state_sensors() == DS_STATE_FL) || (ds_get_state_sensors() == DS_STATE_FC)){
          robot_state = SEEK_AND_DESTROY;
        } else {
          if((millis() - timer) >= MSM_SEEK_DURATION) {
            robot_state = SEEK_AND_DESTROY;
          }
        }
  } else if(robot_state == SEEK_RIGHT) {
        if((ds_get_state_sensors() == DS_STATE_FR) || (ds_get_state_sensors() == DS_STATE_FC)){
          robot_state = SEEK_AND_DESTROY;
        } else {
          if((millis() - timer) >= MSM_SEEK_DURATION) {
            robot_state = SEEK_AND_DESTROY;
          }
        }
  } else {
    robot_state = SEEK_AND_DESTROY;
  }
}

void robot_report_state_name(int state) {
  Serial.print(F("ROBOT_"));
  switch(state) {
    case STARTUP: Serial.print(F("STARTUP")); break;
    case STARTUP_DELAY: Serial.print(F("STARTUP_DELAY")); break;
    case BEGIN: Serial.print(F("BEGIN")); break;
    case SEEK_AND_DESTROY: Serial.print(F("SEEK_AND_DESTROY")); break;
    case SEEK_LEFT: Serial.print(F("SEEK_LEFT")); break;
    case SEEK_RIGHT: Serial.print(F("SEEK_RIGHT")); break;

    case PANIC_FRONT: Serial.print(F("PANIC_FRONT")); break;
    case PANIC_FRONT_STEP_2: Serial.print(F("PANIC_FRONT_STEP_2")); break;
    case PANIC_FRONT_STEP_3: Serial.print(F("PANIC_FRONT_STEP_3")); break;

    case PANIC_REAR: Serial.print(F("PANIC_REAR")); break;
    case PANIC_REAR_STEP_2: Serial.print(F("PANIC_REAR_STEP_2")); break;
    case PANIC_REAR_STEP_3: Serial.print(F("PANIC_REAR_STEP_3")); break;
  }
}

bool robot_report_state_change_name(void){
  if(robot_state != robot_state_old) {
    robot_state_old = robot_state;
    robot_report_state_name(robot_state);
    return true;
  }
  return false;
}