/*!
 * @file WaterLevelReceiver.ino
 *
 * Read out the WaterLevelTransmitter connected on Serial2 and publish the values.
 * 
 * This version publishes on:
 * - serial
 * - a character-based LCD display using the hd44780 chip
 *
 * Tested on an ESP32 DevKitC (Espressif).
 *
 * @author Martin Vanbrabant
 */

#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
#include <Arduino_JSON.h>
#include "LCDbarGraph.h"

// 1: debug output on serial, 0: no debug output 
#define DEBUG 0

// LCD info
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

// An LCD device is connected via I2C, the I2C address is known,
// so using this constructor of this derived class.
hd44780_I2Cexp lcd(LCD_ADDR);

// The bar graph to display the percentage
LCDbarGraph barGraph(lcd, 0, 0, 16, 0, 100);

#define TRIGGER_INTERVAL 1000ul // milliseconds
unsigned long t0;
unsigned long t1;

uint8_t iInputBuffer;
#define BUFFER_LENGTH 256
char inputBuffer[BUFFER_LENGTH];

uint8_t iHistory;
uint8_t nHistory;
#define HISTORY_LENGTH 10
int historyVol_l[HISTORY_LENGTH];
double historyVol_percent[HISTORY_LENGTH];

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  // be patient with serial port 2:
  // wait until initialised and then flush rubbish input
  delay(1000);
  while (Serial2.available() > 0 ) {
    Serial2.read();
  }
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.setCursor(4, 1);
  lcd.print("%");
  lcd.setCursor(15, 1);
  lcd.print("l");

  t0 = millis();
  iInputBuffer = 0;
  iHistory = 0;
  nHistory = 0;

#if DEBUG == 1
  //consume some fake data; useful if not connected to the transmitter
  strcpy(inputBuffer, "{\"t_C\": -15.00, \"distance_m\": 1.600, \"height_m\": 0, \"vol_l\": 0, \"vol_percent\": 0, \"low\": true}");
  for (uint8_t i = 0 ; i < HISTORY_LENGTH ; i++) {
    consume();
    delay(1000);
  }
  strcpy(inputBuffer, "{ \"t_C\": 21.78, \"distance_m\": 0.790, \"height_m\": 1.340, \"vol_l\": 2680, \"vol_percent\": 89.35, \"low\": false}");
  for (uint8_t i = 0 ; i < HISTORY_LENGTH ; i++) {
    consume();
    delay(1000);
  }
#endif  
}

bool timeToTrigger() {
  t1 = millis();
  return (t1 - t0 >= TRIGGER_INTERVAL);
}

void trigger() {
    Serial2.print("?");
    t0 = t1;
    iInputBuffer = 0;
}

void collect() {
  if (Serial2.available() > 0) {
    char c;
    c = Serial2.read();
    if (iInputBuffer == 0) {
      if (c == '{') {
        inputBuffer[iInputBuffer++] = c;
      }
    } else if (iInputBuffer < BUFFER_LENGTH - 1) {
      inputBuffer[iInputBuffer++] = c;
      if (c == '}') {
        inputBuffer[iInputBuffer] = '\0';
        consume();
        iInputBuffer = 0;
      }
    }
  }
}

void consume() {
#if DEBUG == 1
  Serial.println(t1);
  Serial.println(inputBuffer);
#endif

  JSONVar myObject = JSON.parse(inputBuffer);
  if (JSON.typeof(myObject) != "object" ||
      !myObject.hasOwnProperty("vol_l") ||
      !myObject.hasOwnProperty("vol_percent") ||
      !myObject.hasOwnProperty("low")) {
#if DEBUG == 1
    Serial.println("Unexpected input skipped.");
#endif
    return;
  }
 
  historyVol_l[iHistory] = (int) myObject["vol_l"];
  historyVol_percent[iHistory] = (double) myObject["vol_percent"];
  iHistory = (iHistory + 1) % HISTORY_LENGTH;
  if (nHistory < HISTORY_LENGTH) {
    nHistory++;
  }
  long longVol_l = 0;
  for (uint8_t i = 0 ; i < nHistory ; i++) {
    longVol_l += historyVol_l[i];
  }
  int vol_l = longVol_l / nHistory;
  double doubleVol_percent = 0;
  for (uint8_t i = 0 ; i < nHistory ; i++) {
    doubleVol_percent += historyVol_percent[i];
  }
  int vol_percent = doubleVol_percent / nHistory + 0.5;
  
  bool low = (bool) myObject["low"];

  publishOnSerial(vol_l, vol_percent, low);
  publishOnLCD(vol_l, vol_percent, low);
}

void publishOnSerial(int vol_l, int vol_percent, bool low) {
  Serial.printf("vol_l = %d l\n", vol_l);
  Serial.printf("vol_percent = %d %%\n", vol_percent);
  Serial.printf("low = %s\n", low ? "true" : "false");
}

void publishOnLCD(int vol_l, int vol_percent, bool low) {
  static int prevVol_l = -1;
  static int prevVol_percent = -1;
  static bool prevLow = false;

  if (vol_l != prevVol_l) {
    lcd.setCursor(9, 1);
    lcd.printf("%5d", vol_l);
    prevVol_l = vol_l;
  }
  if (vol_percent != prevVol_percent) {
    lcd.setCursor(0, 1);
    lcd.printf("%3d", vol_percent);
    barGraph.display(vol_percent);
    prevVol_percent = vol_percent;
  }
  if (low != prevLow) {
    lcd.setCursor(6, 1);
    lcd.print(low ? "LOW" : "   ");
    prevLow = low;
  }
}

void loop() {
  if (timeToTrigger()) {
    trigger();
  } else {
    collect();
  }
}
