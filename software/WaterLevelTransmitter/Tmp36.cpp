/*!
 * @file Tmp36.cpp
 *
 * @author Martin Vanbrabant
 */

#include "Tmp36.h"

Tmp36::Tmp36(uint8_t analogPin, int adcMaxValue, float adcVoltage) {
  _analogPin = analogPin;
  _adcStep = adcVoltage / (adcMaxValue + 1);
  //Serial.print("adcStep: ");
  //Serial.println(_adcStep, 5);
}

float Tmp36::measureTemperature(void) {
  int a = analogRead(_analogPin);
  float v = a * _adcStep;
  float t = (v - 0.5) * 100;
  //Serial.print("analog value: ");
  //Serial.print(a);
  //Serial.print("; analog voltage: ");
  //Serial.print(v, 5);
  //Serial.println(" V");
  return t;  
}
