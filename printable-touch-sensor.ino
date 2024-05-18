/*
 * printable-touch-sensor - Capacitance change detection for touch
 * 
 * Created: 5/8/2021
 * Author : Winry Litwa-Vulcu
 * 
 * NB: I2C/I3C will use 'controller/target' convention if named.
 *     Other busses will use 'leader/follower' convention if named.
 */

#include "printable-touch-sensor.h"

TerminalCommander::Terminal Terminal(&Serial, &Wire);

CMeter Cmeter(CMETER_PIN0_IN, CMETER_PIN0_OUT);
QTouch Qtouch(QTOUCH_TPIN0, QTOUCH_TPIN1);

// for storing elapsed time in milliseconds since power-on (rolls over after about 50 days)
uint32_t previous_millis = 0;

void qtouch_update(char* args, size_t args_size) {
  Qtouch.loop();
}

void cmeter_update(char* args, size_t args_size) {
  // if (args == nullptr || args_size == 0) {
  //   Serial.println(F("Error: No C index specified"));
  //   return;
  // }

  Cmeter.reset();
  Cmeter.loop();
}

/*! @brief  Called once prior to loop() following power-on or reset
 *
 * @details Detailed description here.
 * @todo    Bring in improvements from motor-control project
 */
void setup() {
  // initialize serial console and set baud rate
  Serial.setTimeout(TERM_TIMEOUT_MILLISECONDS);
  Serial.begin(TERM_BAUD_RATE);

  // initialize Wire library and set clock rate to 400 kHz
  Wire.begin();
  Wire.setClock(I2C_CLK_RATE);

  Qtouch.init();
  Cmeter.init();

  // using a pointer to a function that matches type TerminalCommander::user_callback_fn_t
  Terminal.onCommand("qtouch", &qtouch_update);

  // using a pointer to a function that matches type TerminalCommander::user_callback_fn_t
  Terminal.onCommand("cmeter", &cmeter_update);
}

/*! @brief  An infinite for(;;) loop within Arduino's main()
 *
 * @details Detailed description here.
 * @todo    Bring in improvements from motor-control project
 */
void loop() {
  Terminal.loop();

  // get the elapsed time, in millisecionds, since power-on
  uint32_t current_millis = millis();

  // do this block only every X milliseconds
  if ((current_millis - previous_millis) >= LOOP_INTERVAL_MS) {
    previous_millis = current_millis;

    // low-priority code here    
  }
}
