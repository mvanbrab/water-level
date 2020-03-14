# Introduction
Here I share hardware and software about my **water-level** project,
whose purpose is to measure the water level in a water reservoir,
located under the ground.

Further reading:
* [The physical setup](doc/physical.md)

# The transmitter
The first piece of hardware is a transmitter.
It measures the water level with a waterproof ultrasonic sensor and sends the measurement result as a JSON object via RS232 to a possibly connected receiver.

In the folder **hardware/transmitter** you'll find the hardware design as a [KiCad](https://www.kicad-pcb.org/) project.
Without Kicad installed, you can view [the schematic diagram as .pdf](../master/hardware/transmitter/plot_files/transmitter-schema.pdf).

The folder **software/WaterLevelTransmitter** is an [Arduino](https://www.arduino.cc/) sketch, containing the software of the project.

Further reading:
* [The WaterLevelTransmitter software](doc/water-level-transmitter-software.md)

# The receiver
Currently I do not have a finalised receiver.
At this moment I watch the transmitter's output on a terminal window.
A receiver project will be added later. Stay tuned.



