/*!
 * @file Tmp36.h
 *
 * A TMP36 temperature sensor.
 *
 * Tested on an Arduino Nano.
 * Connect the TMP36 output pin to an analog input.
 * Add a 100 nF capacitor from the analog input to ground.
 *
 * @author Martin Vanbrabant
 */

#ifndef TMP36_H
#define TMP36_H

#include "Arduino.h"

/*! 
 *  Class for measuring temperature using a TMP36
 */
class Tmp36 {
  public:
    /*!
     * Constructor
     * 
     * @param analogPin analog input pin, e.g. AO
     * @param adcMaxValue maximum value of the analog to digital convertor, e.g. 1023 for a 10 bit convertor
     * @param adcVoltage reference voltage of the analog to digital convertor in Volt,
     *               e.g. 5.0 for an Arduino Nano powered via Vin
     *               e.g. 4.62 for an Arduino Nano powered via USB     
     */
    Tmp36(uint8_t analogPin, int adcMaxValue, float adcVoltage);
    /*!
     * Perform one temperature measurement
     * @return the temperature in °C
     */
    float measureTemperature(void);

  private:
    uint8_t _analogPin;
    float _adcStep;
};

#endif
