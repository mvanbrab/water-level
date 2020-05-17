/*!
 * @file WaterLevelTransmitter.ino
 *
 * Measure the water level in a reservoir with an ultrasone distance detector.
 *
 * Tested on an Arduino Nano with a JSN-SR04T-2.0 ultrasone distance detector.
 *
 * @author Martin Vanbrabant
 */

#include "Tmp36.h"
#include "UsDetector.h"

// TMP36
#define TMP36_PIN A7
#define ADC_MAXVALUE 1023
#define ADC_VOLTAGE 5.0

// JSN-SR04T-2.0
#define US_TRIGPIN 7
#define US_ECHOPIN 8
#define US_TIMEOUT 100000ul // microseconds; note JSN-SR04T 2.0 limits to 38 ms automatically

Tmp36 tmp = Tmp36(TMP36_PIN, ADC_MAXVALUE, ADC_VOLTAGE);
UsDetector det = UsDetector(US_TRIGPIN, US_ECHOPIN, US_TIMEOUT);

// --- Dimensions
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

float temperature;
float distance;
float height;
float volume;
float percent;

//JSON string
const char *json_part1 =
  "{\n"
  "  \"t_C\": ";
const char *json_part2 = ",\n"
  "  \"distance_m\": ";
const char *json_part3 = ",\n"
  "  \"height_m\": ";
const char *json_part4 = ",\n"
  "  \"vol_l\": ";
const char *json_part5 = ",\n"
  "  \"vol_percent\": ";
const char *json_part6 = ",\n"
  "  \"low\": ";
const char *json_part7 = "\n"
  "}";

void print_json() {
  Serial.print(json_part1);
  Serial.print(temperature, 2);
  Serial.print(json_part2);
  Serial.print(distance, 3);
  Serial.print(json_part3);
  Serial.print(height, 3);
  Serial.print(json_part4);
  Serial.print(volume, 0);
  Serial.print(json_part5);
  Serial.print(percent, 2);
  Serial.print(json_part6);
  Serial.print(height < X_MIN ? "true" : "false");
  Serial.println(json_part7);
}

void setup() {
  Serial.begin(115200);
  det.begin();
}

void loop() {
  if (Serial.available()) {
    temperature = tmp.measureTemperature();
    distance = det.measureDistance(temperature);
    height = (H1 + H2 - H3) - distance;
    if (height < 0) {
      height = 0;
    } else if (height > H1) {
      height = H1;
    }
    volume = height / H1 * V;
    percent = height / H1 * 100.0;
    print_json();
    Serial.read();
    delay(100);
  }
}
