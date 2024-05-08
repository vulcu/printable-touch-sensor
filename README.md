# printable-touch-sensor
Relative capacitance change detection

Sketch is written for the Arduino Uno, some QTouch library registers may need
to be changed for other Arduinos (Mega, Nano Every, etc.)

Open a serial terminal to the arduino, and type one of the of following:
- `qtouch` QTouch for Arduino library: https://github.com/jgeisler0303/QTouchADCArduino
- `cmeter` Capacitance meter: https://wordpress.codewrite.co.uk/pic/2014/01/25/capacitance-meter-mk-ii/

To run either of these continuously call `qtouch.loop()` or `cmeter.loop()` from inside loop()
