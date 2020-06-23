# The WaterLevelReceiver software

## Operation

The program repeatedly sends a single "?" character to the Serial2 port, at 115200 Baud.
Each "?" acts as a request for a new measurement to the connected transmitter.
If all's well, the transmitter will reply with a JSON output as document in the [transmitter documentation](water-level-transmitter-software.md).

The receiver will decode this JSON payload and publish it to several destinations.
Currently, these are the supported destinations:
- Serial port;
- LCD display connected to the I2C interface, at address 0x27.

## Serial port output

Serial port output is simple textual output of the decode values of the volume in liters, the volume as a percentage and the low indicator.

## LCD output

LCD output consist of:
- a representation of the volume as percentage in a bar graph;
- the volume as percentage in numerical form;
- the volume in liters in numerical form;
- a LOW indicator, only displayed if the level is low.

## Testing

Recompiling the code after changing the preprocessor value of DEBUG to 1 will result in:
- some fake JSON payloads being published first;
- all input received at Serial2 being echoed to Serial.