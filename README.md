# Introduction
Here I share hardware and software about my **water-level** project,
whose purpose is to measure the water level in a water reservoir,
located under the ground.

# Common documentation
Common documentation is available in the **doc** folder of this project:
* [The physcial setup](doc/physical.md)

# The transmitter
The first piece of hardware is a transmitter.
It measures the water level and sends a measurement result as a JSON object via RS232 to a possibly connected receiver.

In the folder **hardware/transmitter** you'll find the hardware design as a [KiCad](https://www.kicad-pcb.org/) project.
Without Kicad installed, you can view [the schematic diagram as .pdf](../blob/master/hardware/transmitter/plot_files/transmitter-schema.pdf).

The folder **software/WaterLevelTransmitter** is an [Arduino](https://www.arduino.cc/) sketch, containing the software of the project.

Further references:
* http://www.beckymarshall.com/depthSensor.html (Experiment 2)
* https://en.wikipedia.org/wiki/Speed_of_sound

# The receiver
Currently I do not have finalised receiver.
At this moment I use a prototype board with an Arduino Nano, and I watch the transmitter's output on the terminal window.
A receiver project will be added once the transmitter has proven to work well in practice and over time.



