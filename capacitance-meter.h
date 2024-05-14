// https://github.com/jgeisler0303/QTouchADCArduino

#pragma once

#include "Arduino.h"

class CMeter {
  public:
    struct result_t {
      float capacitance;
      uint32_t chargeTime;
      uint32_t dischargeTime;
      uint16_t adc;
      bool isLargeCapValue;
    }result;

    CMeter(uint8_t in_pin, uint8_t out_pin);

    void init(void);
    void reset(void);
    void loop(void);

  private:
    const unsigned int in_pin;
    const unsigned int out_pin;

    //Capacitance between this->in_pin and Ground
    //Stray capacitance value will vary from board to board.
    //Calibrate this value using known capacitor.
    const float IN_STRAY_CAP_TO_GND = 24.48;
    const float IN_CAP_TO_GND  = IN_STRAY_CAP_TO_GND;
    //Pullup resistance will vary depending on board.
    //Calibrate this with known capacitor.
    const float R_PULLUP = 34.8;  //in k ohms
    const int   MAX_ADC_VALUE = 1023;
};
