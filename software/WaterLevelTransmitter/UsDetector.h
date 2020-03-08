/*!
 * @file UsDetector.h
 *
 * An ultrasone distance detector.
 *
 * Tested on an Arduino Nano with a JSN-SR04T-2.0 ultrasone distance detector.
 *
 * @author Martin Vanbrabant
 */

#ifndef USDETECTOR_H
#define USDETECTOR_H

#include "Arduino.h"

/*! 
 *  Class capable to do ultrasone distance measurements
 */
class UsDetector {
  public:
    /*!
     * Constructor
     * 
     * @param trigPin pin used to send the trigger pulse to the device
     * @param echoPin pin used to read the echo pulse from the device
     * @param timeout time in microseconds; won't wait longer for the start of the echo pulse
     */
    UsDetector(uint8_t trigPin, uint8_t echoPin, unsigned long timeout);
    /*!
     * Call this method once to begin using this class
     */
    void begin(void);
    /*!
     * Perform one pulse length measurement
     *
     * @return the pulse length in microseconds (0 if no pulse)
     */
    unsigned long measurePulseLength(void);
    /*!
     * Convert a pulse length into a distance measurement (no temperature correction)
     *
     * @param pulseLength pulse length as returned from measurePulseLength
     * @return the distance in meter
     */
    float calculateDistance(unsigned long pulseLength);
    /*!
     * Convert a pulse length into a distance measurement (with temperature correction)
     *
     * @param pulseLength pulse length as returned from measurePulseLength
     * @param temperature the temperature in °C
     * @return the distance in meter
     */
    float calculateDistance(unsigned long pulseLength, float temperature);
    /*!
     * Perform one distance measurement (no temperature correction).
     * A convenience method, equivalent to calculateDistance(measurePulseLength())
     *
     * @return the distance in meter
     */
    float measureDistance(void);
    /*!
     * Perform one distance measurement (with temperature correction)
     * A convenience method, equivalent to calculateDistance(measurePulseLength(), temperature)
     *
     * @param temperature the temperature in °C
     * @return the distance in meter
     */
    float measureDistance(float temperature);

  private:
    uint8_t _trigPin;
    uint8_t _echoPin;
    unsigned long _timeout;
};

#endif
