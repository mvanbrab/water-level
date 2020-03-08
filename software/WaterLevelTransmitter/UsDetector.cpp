/*!
 * @file UsDetector.cpp
 *
 * See for the bitbanging: http://www.beckymarshall.com/depthSensor.html
 * See for speed of sound vs. temperature: https://en.wikipedia.org/wiki/Speed_of_sound
 *
 * @author Martin Vanbrabant
 */

#include "UsDetector.h"

UsDetector::UsDetector(uint8_t trigPin, uint8_t echoPin, unsigned long timeout) {
  _trigPin = trigPin;
  _echoPin = echoPin;
  _timeout = timeout;
}

void UsDetector::begin(void) {
  pinMode(_trigPin, OUTPUT);
  pinMode(_echoPin, INPUT);
  digitalWrite(_trigPin, LOW);
  measurePulseLength(); // first measurement result is undefined
}

unsigned long UsDetector::measurePulseLength(void) {
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(15); // more then the minimum of 10 us
  digitalWrite(_trigPin, LOW);
  unsigned long p = pulseIn(_echoPin, HIGH, _timeout);
  //Serial.print("Pulse length: ");
  //Serial.print(p);
  //Serial.println(" microseconds");
  return p;
}

float UsDetector::calculateDistance(unsigned long pulseLength) {
	return 340.0 / 2.0e6 * pulseLength;
}

float UsDetector::calculateDistance(unsigned long pulseLength, float temperature) {
	return (331.3 + 0.606 * temperature) / 2.0e6 * pulseLength;
}

float UsDetector::measureDistance(void) {
  return calculateDistance(measurePulseLength());
}

float UsDetector::measureDistance(float temperature) {
  return calculateDistance(measurePulseLength(), temperature);
}
