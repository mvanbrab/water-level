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

uint8_t iInputBuffer;
#define BUFFER_LENGTH 256
char inputBuffer[BUFFER_LENGTH];

// Smoothed measurements
bool firstRun;
double tDegreesCelcius;
double volumeLiter;
double volumePercent;
double low;

// Timing
#define TRIGGER_INTERVAL 1000ul // milliseconds
unsigned long t0;
unsigned long t1;


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
  lcd.setCursor(3, 1);
  lcd.print("%");
  lcd.setCursor(14, 1);
  lcd.print("l");

  iInputBuffer = 0;
  firstRun = true;

#if DEBUG == 1
  //consume some fake data; useful if not connected to the transmitter
  const char * fakeInput[] = {
    "{\"t_C\": -15.00, \"distance_m\": 2.130, \"height_m\": 0.000, \"vol_l\":    0, \"vol_percent\":   0.00, \"low\":  true}",
    "{\"t_C\": -10.00, \"distance_m\": 1.980, \"height_m\": 0.150, \"vol_l\":  300, \"vol_percent\":  10.00, \"low\":  true}",
    "{\"t_C\":  -5.00, \"distance_m\": 1.830, \"height_m\": 0.300, \"vol_l\":  600, \"vol_percent\":  20.00, \"low\": false}",
    "{\"t_C\":   0.00, \"distance_m\": 1.680, \"height_m\": 0.450, \"vol_l\":  900, \"vol_percent\":  30.00, \"low\": false}",
    "{\"t_C\":   5.00, \"distance_m\": 1.530, \"height_m\": 0.600, \"vol_l\": 1200, \"vol_percent\":  40.00, \"low\": false}",
    "{\"t_C\":  10.00, \"distance_m\": 1.380, \"height_m\": 0.750, \"vol_l\": 1500, \"vol_percent\":  50.00, \"low\": false}",
    "{\"t_C\":  15.00, \"distance_m\": 1.230, \"height_m\": 0.900, \"vol_l\": 1800, \"vol_percent\":  60.00, \"low\": false}",
    "{\"t_C\":  20.00, \"distance_m\": 1.080, \"height_m\": 1.050, \"vol_l\": 2100, \"vol_percent\":  70.00, \"low\": false}",
    "{\"t_C\":  21.00, \"distance_m\": 0.930, \"height_m\": 1.200, \"vol_l\": 2400, \"vol_percent\":  80.00, \"low\": false}",
    "{\"t_C\":  21.50, \"distance_m\": 0.780, \"height_m\": 1.350, \"vol_l\": 2700, \"vol_percent\":  90.00, \"low\": false}",
    "{\"t_C\":  21.78, \"distance_m\": 0.630, \"height_m\": 1.500, \"vol_l\": 3000, \"vol_percent\": 100.00, \"low\": false}"
  };
  uint8_t j;
  for (uint8_t i = 0, j = 0 ; i < 120 ; i++) {
    consume(fakeInput[j]);
    if (j < sizeof(fakeInput)/sizeof(fakeInput[0]) - 1) {
      j++;
    }
    delay(1000);
  }
#endif  

  t0 = millis();
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
        consume(inputBuffer);
        iInputBuffer = 0;
      }
    } else {
      // oops, avoid buffer overflow
      iInputBuffer = 0;
    }
  }
}

void consume(const char * inputString) {
#if DEBUG == 1
  Serial.println(t1);
  Serial.println(inputString);
#endif

  JSONVar myObject = JSON.parse(inputString);
  if (JSON.typeof(myObject) != "object" ||
      !myObject.hasOwnProperty("t_C") ||
      !myObject.hasOwnProperty("vol_l") ||
      !myObject.hasOwnProperty("vol_percent") ||
      !myObject.hasOwnProperty("low")) {
#if DEBUG == 1
    Serial.println("Unexpected input skipped.");
#endif
    return;
  }

  smooth(tDegreesCelcius, (double)myObject["t_C"]);
  smooth(volumeLiter,     (double)((int) myObject["vol_l"]));
  smooth(volumePercent,   (double)myObject["vol_percent"]);
  smooth(low,             (double)((bool) myObject["low"]));
  firstRun = false;

  int volumeLiterInt = volumeLiter + 0.5;
  int volumePercentInt = volumePercent + 0.5;
  bool lowBool = (low >= 0.5);

  publishOnSerial(tDegreesCelcius, volumeLiterInt, volumePercentInt, lowBool);
  publishOnLCD   (tDegreesCelcius, volumeLiterInt, volumePercentInt, lowBool);
}

void smooth(double& smoothed, double raw) {
  if (firstRun) {
    smoothed = raw;
  } else {
    smoothed = 0.9 * smoothed + 0.1 * raw;
  }
}

void publishOnSerial(double tDegreesCelcius, int volumeLiterInt, int volumePercentInt, bool lowBool) {
  char buf[100];
  sprintf(buf, "t = %lf °C", tDegreesCelcius);
  Serial.println(buf);
  sprintf(buf, "volume = %d l (%d %%)%s", volumeLiterInt, volumePercentInt, lowBool ? " LOW!" : "");
  Serial.println(buf);
}

void publishOnLCD(double tDegreesCelcius, int volumeLiterInt, int volumePercentInt, bool lowBool) {
  static int prevVolumeLiterInt = -1;
  static int prevVolumePercentInt = -1;
  static bool prevLowBool = false;
  static const char hartBeat[] = {'*', ' '};
  static uint8_t iHartBeat = 0;

  if (volumeLiterInt != prevVolumeLiterInt) {
    lcd.setCursor(9, 1);
    lcd.printf("%5d", volumeLiterInt);
    prevVolumeLiterInt = volumeLiterInt;
  }
  if (volumePercentInt != prevVolumePercentInt) {
    lcd.setCursor(0, 1);
    lcd.printf("%3d", volumePercentInt);
    barGraph.display(volumePercentInt);
    prevVolumePercentInt = volumePercentInt;
  }
  if (lowBool != prevLowBool) {
    lcd.setCursor(5, 1);
    lcd.print(lowBool ? "LOW" : "   ");
    prevLowBool = lowBool;
  }
  lcd.setCursor(15, 1);
  lcd.write(hartBeat[iHartBeat]);
  iHartBeat = (iHartBeat + 1) % (sizeof(hartBeat) / sizeof(hartBeat[0]));
}

void loop() {
  if (timeToTrigger()) {
    trigger();
  } else {
    collect();
  }
}
