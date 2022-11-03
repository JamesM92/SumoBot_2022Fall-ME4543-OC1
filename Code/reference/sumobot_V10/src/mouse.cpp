/**
 * @file mouse.cpp
 * @author M. Good
 * @brief MOUSE SENSOR (KA2.B PS2 Mouse Sensor similar to A2633)
 * @version 0.1
 * @date 2022-06-11
 * 
 * @copyright Copyright (c) 2022
 * @note 
 * PAW3515DB-VJZA
 * Video and Source Code
 * Measure Movement on your Arduino with a Mouse - How to connect a standard mouse to an Arduino. - https://www.youtube.com/watch?v=j33jA7B2Bz4
 * Arduino-PS2-Mouse-Handler - https://github.com/getis/Arduino-PS2-Mouse-Handler
 * 
 * 3 Button Desktop PS/2 Optical Scroll Mouse (Amazon) - https://www.amazon.com/dp/B003LD0BQ6?ref=ppx_yo2ov_dt_b_product_details&th=1
 * 
 * Wiring from the PS2 mouse purchased from Amazon. Chip KA2.B
 * WHITE  GND   GND
 * BLUE   5V    +5V
 * ORANGE D+    CLOCK
 * GREEN  D-    DATA
 * 
 * Note:
 * Must use PWM pins.
 * PWM Pins Uno, Nano, Mini    3, 5, 6, 9, 10, 11    490 Hz (pins 5 and 6: 980 Hz)
 * PWM Pins Mega               2 - 13, 44 - 46       490 Hz (pins 4 and 13: 980 Hz)
 * 
 */
#include <Arduino.h>
#include <PS2MouseHandler.h>
#include "mouse.h"

#define MOUSE_DATA  5   // WHITE D-/DATA    Mouse D-/DATA to Arduino Pin 5
#define MOUSE_CLOCK 6   // GREEN D+/CLK     Mouse D+/CLK to to Arduino Pin 6

PS2MouseHandler mouse(MOUSE_CLOCK, MOUSE_DATA, PS2_MOUSE_REMOTE);

void mouse_setup(void) {
  Serial.println("mouse_setup");
  mouse.initialise();
  Serial.println("mouse_setup complete");
}

void mouse_report(void) {
  mouse.get_data();
  Serial.print(mouse.x_movement());
  Serial.print(",");
  Serial.println(mouse.y_movement());
}