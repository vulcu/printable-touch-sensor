// https://github.com/jgeisler0303/QTouchADCArduino

#include "qtouch-adc-arduino.h"

QTouch::QTouch(uint8_t tpin0, uint8_t tpin1) : 
    tpin0(tpin0), 
    tpin1(tpin1) { };

void QTouch::init(void) {
  // prepare the ADC unit for one-shot measurements
  // see the atmega328 datasheet for explanations of the registers and values
  ADMUX = 0b01000000; // Vcc as voltage reference (bits76), right adjustment (bit5), use ADC0 as input (bits3210)
  ADCSRA = 0b11000100; // enable ADC (bit7), initialize ADC (bit6), no autotrigger (bit5), don't clear int-flag  (bit4), no interrupt (bit3), clock div by 16@16Mhz=1MHz (bit210) ADC should run at 50kHz to 200kHz, 1MHz gives decreased resolution
  ADCSRB = 0b00000000; // autotrigger source free running (bits210) doesn't apply
  while(ADCSRA & (1<<ADSC)){  } // wait for first conversion to complete  
}

void QTouch::loop(void) {
  unsigned long time;
  time = micros();
  
  // 4 measurements are taken and averaged to improve noise immunity
  for (int i=0; i<4; i++) {
    // first measurement: charge touch probe, discharge ADC s&h cap, connect the two, measure the volage
    this->adc0 += this->probe(this->tpin0, this->tpin1, false); // accumulate the results for the averaging

    // second measurement:discharge touch probe, charge ADC s&h cap, connect the two, measure the volage
    this->adc1 += this->probe(this->tpin0, this->tpin1, true); // accumulate the results for the averaging
  }
  this->adc0 >>= 2; // divide the accumulated measurements by 16
  this->adc1 >>= 2;

  time= micros() - time;
 
  // resulting raw touch value
  probe_val= adc0-adc1; // the value of adc0 (probe charged) gets higher when the probe ist touched, the value of adc1 (s&h charged) gets lower when the probe ist touched, so, it has to be be subtracted to amplify the detection accuracy
  
  // calculate the index to the LED fading table
  int16_t idx= (this->probe_val - TOUCH_VALUE_BASELINE); // offset probe_val by value of untouched probe
  if(idx<0) idx = 0; // limit the index!!!
  idx /= TOUCH_VALUE_SCALE; // scale the index
  if(idx>31) idx = 31; // limit the index!!!

  // print some info to the serial
  Serial.print(time);
  Serial.print('|');
  Serial.print(adc0);
  Serial.print('|');
  Serial.print(adc1);
  Serial.print('|');
  Serial.print(probe_val);
  Serial.print('|');
  Serial.println(idx);
  
  // fade the LED
  analogWrite(9, this->ledFadeTable[idx]);
  
  this->adc0 = 0; // clear the averaging variables for the next run
  this->adc1 = 0;
}

uint16_t QTouch::probe(uint8_t pin, uint8_t partner, bool dir) {
  uint8_t mask= _BV(pin) | _BV(partner);
  
  DDRC|= mask; // config pins as push-pull output
  if(dir)
    PORTC= (PORTC & ~_BV(pin)) | _BV(partner); // set partner high to charge the  s&h cap and pin low to discharge touch probe cap
  else
    PORTC= (PORTC & ~_BV(partner)) | _BV(pin); // set pin high to charge the touch probe and pin low to discharge s&h cap cap
  
  // charge/discharge s&h cap by connecting it to partner
  ADMUX = MUX_REF_VCC | partner; // select partner as input to the ADC unit
  
  delayMicroseconds(CHARGE_DELAY); // wait for the touch probe and the s&h cap to be fully charged/dsicharged
  
  DDRC&= ~mask; // config pins as input
  PORTC&= ~mask; // disable the internal pullup to make the ports high Z
  
  // connect touch probe cap to s&h cap to transfer the charge
  ADMUX= MUX_REF_VCC | pin; // select pin as ADC input
  
  delayMicroseconds(TRANSFER_DELAY); // wait for charge to be transfered
  
  // measure
  ADCSRA|= (1<<ADSC); // start measurement
  while(ADCSRA & (1<<ADSC)){  } // wait for conversion to complete
  return ADC; // return conversion result
}  
