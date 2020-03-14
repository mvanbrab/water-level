# The WaterLevelTransmitter software

## Operation

The program waits for serial input, expected at 115200 Baud.
The contents of the serial input don't care.

As soon as serial input is received, one measurement is executed.
First, the time for an ultrasonic burst to travel from the sensor to the water surface and to return is then measured.
Next, from this time, taking into account temperature, the distance (and derived variables) are then calculated.
The result is packaged in a [JSON](https://tools.ietf.org/html/rfc7158) text, making it ready for human as well as machine consumption.

Finally, any pending serial input that might be received in the mean time, is flushed and after a short delay, new serial input is expected, restarting the entire cycle.

## The JSON output

An example:
```
{
  "t_C": 21.78,
  "distance_m": 0.790,
  "height_m": 1.340,
  "vol_l": 2680,
  "vol_percent": 89.35,
  "low": false
}
```

Another example, this time with an active *low* indication:
```
{
  "t_C": 22.27,
  "distance_m": 1.968,
  "height_m": 0.162,
  "vol_l": 325,
  "vol_percent": 10.83,
  "low": true
}
```

Properties in the JSON object:

| property name | value |
| ------------- | ----- |
| "t_C" | temperature, in °C |
| "distance_m" | distance from sensor to water surface (d), in meter |
| "height_m" | water level, heigth measured from the bottom (x), in meter |
| "vol_l" | volume of available water, in liter |
| "vol_percent" | volume of available water, % of maximum volume |
| "low" | boolean; true means water level below safety threshold |
 
## Configuring the code
The following constants in the code of WaterLevelTransmitter.ino can be adapted to a specific physical setup.
See also [the physical setup](physical.md).

```
// H1 (meter)
#define H1 1.50
// H2 (meter)
#define H2 0.68
// H3 (meter)
#define H3 0.05
// V (liter)
#define V 3000
// X_MIN (meter)
#define X_MIN 0.25
// X_MIN (meter)
#define X_MIN 0.25
```

## Testing
This program can be easily tested by connecting the Arduino Nano's USB port to a terminal program, such as the Serial Monitor
included in the Arduino IDE.

Set the baudrate to 115200.

Set line ending mode to Newline.

Every push on the "Send" button, even with empty input line, results in a JSON text returned by the board.

## References
* http://www.beckymarshall.com/depthSensor.html (Experiment 2)
  * I based the bit banging code to control the JSN-SR04T-2.0 ultrasone distance detector upon her well working experimental code.
* https://en.wikipedia.org/wiki/Speed_of_sound
  * I used the given formula for calculating the speed of sound as a function of temperature in °C (c_air = 331.3 + 0.606 * t_C) from this reference. 