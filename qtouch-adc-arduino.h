// https://github.com/jgeisler0303/QTouchADCArduino

#pragma once

#include "Arduino.h"

// ADC constants
#define ADMUX_MASK  0b00001111 // mask the mux bits in the ADMUX register
#define MUX_GND 0b00001111 // mux value for connecting the ADC unit internally to GND
#define MUX_REF_VCC 0b01000000 // value to set the ADC reference to Vcc

#define CHARGE_DELAY  5 // time it takes for the capacitor to get charged/discharged in microseconds
#define TRANSFER_DELAY  5 // time it takes for the capacitors to exchange charge
#define TOUCH_VALUE_BASELINE -165 // this is the value my setup measures when the probe is not touched. For your setup this might be different. In order for the LED to fade correctly, you will have to adjust this value
#define TOUCH_VALUE_SCALE 5 // this is also used for the LED fading. The value should be chosen such that the value measured when the probe is fully touched minus TOUCH_VALUE_BASELINE is scaled to 31, e.g. untouched_val= 333, touched_val= 488, difference= 155, divide by 5 to get 31.

class QTouch {
  public:
    QTouch(uint8_t tpin1, uint8_t tpin2);

    void init(void);
    void loop(void);

  private:
    unsigned int adc0, adc1; // store the avarage of the charge resp. discharge measurement
    unsigned int tpin0, tpin1;
    int probe_val; // store the resulting touch measurement
    const uint16_t ledFadeTable[32] = {0, 1, 1, 2, 2, 2, 3, 3, 4, 5, 6, 7, 9, 10, 12, 15, 17, 21, 25, 30, 36, 43, 51, 61, 73, 87, 104, 125, 149, 178, 213, 255}; // this is an exponential series to model the perception of the LED brightness by the human eye

    uint16_t probe(uint8_t pin, uint8_t partner, bool dir);
};
