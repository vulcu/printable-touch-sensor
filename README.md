# printable-touch-sensor

Relative capacitance change detection

## Installation

```shell
# clone into a subdirectory in your Arduino sketch directory
cd <path to your top-level Arduino sketch directory>
git clone https://github.com/vulcu/printable-touch-sensor.git

# initialize the submodules used by this repository
git submodule update --init
```

Sketch is written for the Arduino Uno, some QTouch library registers may need
to be changed for other Arduinos (Mega, Nano Every, etc.)

## Use

Open a serial terminal to the arduino, and type one of the of following:

- `qtouch` QTouch for Arduino library: [QTouchADCArduino](https://github.com/jgeisler0303/QTouchADCArduino)
- `cmeter` Capacitance meter: [Capacitance Meter](https://wordpress.codewrite.co.uk/pic/2014/01/25/capacitance-meter-mk-ii/)

To run either of these continuously call `qtouch.loop()` or `cmeter.loop()` from inside loop()
