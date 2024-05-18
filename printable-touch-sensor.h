/*
 * printable-touch-sensor.h - Capacitance change detection for touch
 * 
 * Created: 5/8/2021
 * Author : Winry Litwa-Vulcu
 */

#ifndef PRINTABLE_TOUCH_SENSOR_H
#define PRINTABLE_TOUCH_SENSOR_H

  #include <Wire.h>

  #include "lib/terminal-commander/terminal_commander.h"
  #include "lib/terminal-commander/terminal_commander.cpp"

  #include "capacitance-meter.h"
  #include "qtouch-adc-arduino.h"

  // Timing interval in milliseconds for running secondary tasks in main loop
  #define LOOP_INTERVAL_MS (100UL)

  // I2C communication bus speed
  #define I2C_CLK_RATE  (400000L)

  // UART serial console communication baud rate and timeout
  #define TERM_BAUD_RATE            (115200L)
  #define TERM_TIMEOUT_MILLISECONDS (10U)

  // CMeter pins
  #define CMETER_PIN0_IN  (A0)
  #define CMETER_PIN0_OUT (A2)

  // QTouch pins
  #define QTOUCH_TPIN0  (0U)
  #define QTOUCH_TPIN1  (1U)

#endif
