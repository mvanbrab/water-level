# Introduction
Here I share hardware and software about my **water-level** project,
whose purpose is to measure the water level in a water reservoir,
located under the ground.

This project actually has a predecessor. Enjoy reading about it in this [bit of history](doc/history.md).

# The transmitter
The first piece of hardware is a transmitter.
It measures the water level with a waterproof ultrasonic sensor and sends the measurement result as a JSON object via RS232 to a possibly connected receiver.

The positioning of this sensor in my project is described in the [physical setup](doc/physical.md).

Here are some [pictures](doc/pictures-transmitter.md) showing the transmitter in practice.

In the folder **hardware/transmitter** you'll find the hardware design as a [KiCad](https://www.kicad-pcb.org/) project.
Without Kicad installed, you can view
- [the schematic diagram as .pdf](../master/hardware/transmitter.pdf);
- [the copper layout of the printed circuit board](../master/hardware/transmitter_cu.pdf);
- [the component placement on the printed circuit board](../master/hardware/transmitter_silk.pdf).

The folder **software/WaterLevelTransmitter** is an [Arduino](https://www.arduino.cc/) sketch, containing the software of the transmitter part of the project.

Read more about this piece of software [here](doc/water-level-transmitter-software.md).

# The receiver
The second piece of hardware is the receiver.
It requests output from the transmitter and publishes is to whatever destination.

Here are some [pictures](doc/pictures-receiver.md) showing the receiver in practice.

In the folder **hardware/receiver** you'll find the hardware design as a [KiCad](https://www.kicad-pcb.org/) project.
Without Kicad installed, you can view
- [the schematic diagram as .pdf](../master/hardware/receiver.pdf);
- [the copper layout of the printed circuit board](../master/hardware/receiver_cu.pdf);
- [the component placement on the printed circuit board](../master/hardware/receiver_silk.pdf).

About the receiver hardware:
- the RS232 level converter is the same as used for the transmitter, this time it is powered from 3V3;
- the character-based LCD screen to connect is a 2 rows of 16 columns type, using an HD44780 driver, connected via I2C @ 3V3, but powered @ 5V;
- the processor board (chosen because of its WiFi connectivity) is a [ESP32 DevKitC](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html)
 (with ESP32-WROOM-32D module).

The folder **software/WaterLevelReceiver** is an [Arduino](https://www.arduino.cc/) sketch, containing the software of the receiver part of the project.

Read more about this piece of software [here](doc/water-level-receiver-software.md).

# Lessons learned
[The temperature measurement](doc/ll01.md)




