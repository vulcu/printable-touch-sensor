// https://github.com/jgeisler0303/QTouchADCArduino

#include "capacitance-meter.h"

CMeter::CMeter(uint8_t in_pin, uint8_t out_pin) : 
  in_pin(in_pin), 
  out_pin(out_pin) { };

void CMeter::init(void) {
  pinMode(this->out_pin, OUTPUT);
  digitalWrite(this->out_pin, LOW);

  pinMode(this->in_pin, OUTPUT);
  digitalWrite(this->in_pin, LOW);
}

void CMeter::reset(void) {
  this->init();
  delay(1);
}

void CMeter::loop(void) {
  //Capacitor under test between out_pin and in_pin
  //Rising high edge on out_pin
  pinMode(this->in_pin, INPUT);
  digitalWrite(this->out_pin, HIGH);
  int val = analogRead(this->in_pin);
  digitalWrite(this->out_pin, LOW);

  if (val < 1000) {
    //Low value capacitor
    //Clear everything for next measurement
    pinMode(this->in_pin, OUTPUT);
    digitalWrite(this->in_pin, LOW);

    //Calculate and print result

    float capacitance = (float)val * this->IN_CAP_TO_GND / (float)(this->MAX_ADC_VALUE - val);

    // Serial.print(F("Capacitance Value = "));
    // Serial.print(capacitance, 3);
    // Serial.print(F(" pF ("));
    // Serial.print(val);
    // Serial.println(F(") "));
  }
  else {
    //Big capacitor - so use RC charging method

    //discharge the capacitor (from low capacitance test)
    pinMode(this->in_pin, OUTPUT);
    digitalWrite(this->in_pin, LOW);
    delay(1);

    //Start charging the capacitor with the internal pullup
    pinMode(this->out_pin, INPUT_PULLUP);
    unsigned long u1 = micros();
    unsigned long t;
    int digVal;

    //Charge to a fairly arbitrary level mid way between 0 and 5V
    //Best not to use analogRead() here because it's not really quick enough
    do {
      digVal = digitalRead(this->out_pin);
      unsigned long u2 = micros();
      t = u2 > u1 ? u2 - u1 : u1 - u2;
    } while ((digVal < 1) && (t < 400000L));

    pinMode(this->out_pin, INPUT);  //Stop charging
    //Now we can read the level the capacitor has charged up to
    val = analogRead(this->out_pin);

    //Discharge capacitor for next measurement
    digitalWrite(this->in_pin, HIGH);
    int dischargeTime = (int)(t / 1000L) * 5;
    delay(dischargeTime);    //discharge slowly to start with
    pinMode(this->out_pin, OUTPUT);  //discharge remainder quickly
    digitalWrite(this->out_pin, LOW);
    digitalWrite(this->in_pin, LOW);

    //Calculate and print result
    float capacitance = -(float)t / this->R_PULLUP / log(1.0 - (float)val / (float)this->MAX_ADC_VALUE);

    // Serial.print(F("Capacitance Value = "));
    // if (capacitance > 1000.0) {
    //   Serial.print(capacitance / 1000.0, 2);
    //   Serial.print(F(" uF"));
    // }
    // else {
    //   Serial.print(capacitance, 2);
    //   Serial.print(F(" nF"));
    // }

    // Serial.print(F(" ("));
    // Serial.print(digVal == 1 ? F("Normal") : F("HighVal"));
    // Serial.print(F(", t= "));
    // Serial.print(t);
    // Serial.print(F(" us, ADC= "));
    // Serial.print(val);
    // Serial.println(F(")"));
  }
}
