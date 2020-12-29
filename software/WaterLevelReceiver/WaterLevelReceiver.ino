/*!
 * @file WaterLevelReceiver.ino
 *
 * Read out the WaterLevelTransmitter connected on Serial2 and publish the values.
 * 
 * This version publishes on:
 * - the serial port accessible through the USB connection
 * - a character-based LCD display using the hd44780 chip
 * - a channel on https://thingspeak.com
 *
 * Tested on an ESP32 DevKitC (Espressif).
 *
 * @author Martin Vanbrabant
 */

#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
#include <Arduino_JSON.h>
#include <EspMQTTClient.h>
#include "LCDbarGraph.h"
// Create your own secrets.h (mine is not in the repository), based on secrets_template.h
#include "secrets.h"

// 1: debug output (general) on serial, 0: no debug output (general)
#define DEBUG 0
// 1: debug output (MQTT) on serial, 0: no debug output (MQTT)
#define DEBUG_MQTT 1
// 1: start by processing some fake input, 0: no fake input
#define DEBUG_FAKE_INPUT 0

// IO pins
#define BACKLIGHT_BUTTON_IOPIN 12

// LCD info
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2
#define LCD_BAR_X 0
#define LCD_BAR_Y 0
#define LCD_BAR_L 16
#define LCD_PERCENT_VALUE_X 0
#define LCD_PERCENT_VALUE_Y 1
#define LCD_LOW_VALUE_X 5
#define LCD_LOW_VALUE_Y 1
#define LCD_L_VALUE_X 9
#define LCD_L_VALUE_Y 1
#define LCD_HEARTBEAT_X 15
#define LCD_HEARTBEAT_Y 1

// An LCD device is connected via I2C, the I2C address is known,
// so using this constructor of this derived class.
hd44780_I2Cexp lcd(LCD_ADDR);

// The bar graph to display the percentage
LCDbarGraph barGraph(lcd, LCD_BAR_X, LCD_BAR_Y, LCD_BAR_L, 0, 100);

// The status of the backlight
bool backlightOn;

// Timing - everything unsigned long, milliseconds
#define INITIAL_SERIAL2_DELAY 1000ul
#define INITIAL_TRIGGER_DELAY 15000ul
#define TRIGGER_INTERVAL 1000ul
#define MQTT_INTERVAL 60000ul
#define BUTTON_DEBOUNCE_DELAY 100ul

// MQTT
EspMQTTClient mqttClient(
  SECRET_SSID,
  SECRET_WIFI_PASS,
  SECRET_MQTT_SERVER,     // MQTT Broker server ip
  SECRET_MQTT_USER,       // Can be omitted if not needed
  SECRET_MQTT_PASS,       // Can be omitted if not needed
  "mvanbrab-waterlevel",  // Client name that uniquely identifies your device
  SECRET_MQTT_PORT        // The MQTT port, default to 1883.
);

/*!
 * This function is called once everything is connected (Wifi and MQTT)
 * WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
 */
void onConnectionEstablished()
{
#if DEBUG_MQTT == 1
  Serial.println("MQTT connected.");
#endif
}

/*!
 * Arduino's setup function
 */
void setup() {
  pinMode(BACKLIGHT_BUTTON_IOPIN, INPUT_PULLUP);

  Serial.begin(115200);
  Serial2.begin(115200);
  unsigned long tSerial2 = millis();

  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.print("Initialising...");
  backlightOn = true;

  // be patient with serial port 2:
  // wait until initialised and then flush rubbish input
  while (millis() - tSerial2 < INITIAL_SERIAL2_DELAY) {
    // empty
  }
  while (Serial2.available() > 0 ) {
    Serial2.read();
  }

#if DEBUG_MQTT == 1
  mqttClient.enableDebuggingMessages(); // Enable debugging messages sent to serial output
#endif
}

/*!
 * Arduino's loop function
 */
void loop() {
  mqttClient.loop();
#if DEBUG_FAKE_INPUT == 0
  handleTransmitter();
#else
  handleFakeInput();
#endif
  handleButtons();
}

/*!
 * Interact with the WaterLevelTransmitter
 */
void handleTransmitter() {
  static bool first = true;
  static unsigned long tPrev = 0;
  static uint8_t iInputBuffer = 0;
#define BUFFER_LENGTH 256
  static char inputBuffer[BUFFER_LENGTH];
  unsigned long tNow;

  if (first) {
    // ignore the response of the transmitter to initial uncontrolled output on our Serial2
    while (Serial2.available() > 0) {
      Serial2.read();
    }
    // first trigger?
    tNow = millis();
    if (tNow - tPrev >= INITIAL_TRIGGER_DELAY) {
      Serial2.print("?");
      first = false;
      tPrev = tNow;
    }
  } else {
    // handle the response of the transmitter
    while (Serial2.available() > 0) {
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
    // next trigger?
    tNow = millis();
    if (tNow - tPrev >= TRIGGER_INTERVAL) {
      Serial2.print("?");
      tPrev = tNow;
    }
  }
}

#if DEBUG_FAKE_INPUT == 1
/*!
 * Replacement for handleTransmitter() for debugging; useful to test if not connected to the transmitter
 */
void handleFakeInput() {
  static const char * fakeInput[] = {
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
  static int i = 0;
  static int j = 0;
  static unsigned long tPrev = 0;
  unsigned long tNow;

  if (i < 600) {
    tNow = millis();
    if (tNow - tPrev >= TRIGGER_INTERVAL) {
      consume(fakeInput[j]);
      if (j < sizeof(fakeInput)/sizeof(fakeInput[0]) - 1) {
        j++;
      }
      i++;
      tPrev = tNow;
    }
  }
}
#endif

/*!
 * Consume one collected input string
 *
 * @param inputString the collected JSON string
 */
void consume(const char * inputString) {
  static bool first = true;
  static double tDegreesCelcius;
  static double volumeLiter;
  static double volumePercent;
  static double low;

#if DEBUG == 1
  Serial.println(millis());
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

  smooth(tDegreesCelcius, (double)myObject["t_C"],           first);
  smooth(volumeLiter,     (double)((int) myObject["vol_l"]), first);
  smooth(volumePercent,   (double)myObject["vol_percent"],   first);
  smooth(low,             (double)((bool) myObject["low"]),  first);
  first = false;

  bool lowBool = (low >= 0.5);

  publishOnSerial(tDegreesCelcius, volumeLiter, volumePercent, lowBool);
  publishOnLCD(tDegreesCelcius, volumeLiter, volumePercent, lowBool);
  publishOnMqtt(tDegreesCelcius, volumeLiter, volumePercent, lowBool);
}

/*!
 * Smooth a measurement value
 *
 * @param smoothed reference to the smoothed value (to be provided to all next calls)
 * @param raw the new measured value
 * @param first if true, this is the first measurement (previous smoothed value will not be used)
 */
void smooth(double& smoothed, double raw, bool first) {
  if (first) {
    smoothed = raw;
  } else {
    smoothed = 0.9 * smoothed + 0.1 * raw;
  }
}

/*!
 * Publish the measurements to the standard serial port
 *
 * @param tDegreesCelcius temperature in degrees Celcius
 * @param volumeLiter volume in litres
 * @param volumePercent volume as a percentage
 * @param lowBool low indicator
 */
void publishOnSerial(double tDegreesCelcius, double volumeLiter, double volumePercent, bool lowBool) {
  char buf[100];
  sprintf(buf, "t = %lf °C", tDegreesCelcius);
  Serial.println(buf);
  sprintf(buf, "volume = %lf l (%lf %%)%s", volumeLiter, volumePercent, lowBool ? " LOW!" : "");
  Serial.println(buf);
}

/*!
 * Publish the measurements to the LCD display
 *
 * @param tDegreesCelcius temperature in degrees Celcius
 * @param volumeLiter volume in litres
 * @param volumePercent volume as a percentage
 * @param lowBool low indicator
 */
void publishOnLCD(double tDegreesCelcius, double volumeLiter, double volumePercent, bool lowBool) {
  static bool first = true;
  static int prevVolumeLiterInt = -1;
  static int prevVolumePercentInt = -1;
  static bool prevLowBool = false;
  static const char heartBeat[] = {'*', ' '};
  static uint8_t iHeartBeat = 0;

  int volumePercentInt = volumePercent + 0.5;
  if (volumePercentInt != prevVolumePercentInt) {
    lcd.setCursor(LCD_PERCENT_VALUE_X, LCD_PERCENT_VALUE_Y);
    lcd.printf("%3d", volumePercentInt);
    if (first) {
      lcd.print("%");
    }
    barGraph.display(volumePercentInt);
    prevVolumePercentInt = volumePercentInt;
  }
  if (lowBool != prevLowBool) {
    lcd.setCursor(LCD_LOW_VALUE_X, LCD_LOW_VALUE_Y);
    lcd.print(lowBool ? "LOW" : "   ");
    prevLowBool = lowBool;
  }
  int volumeLiterInt = volumeLiter + 0.5;
  if (volumeLiterInt != prevVolumeLiterInt) {
    lcd.setCursor(LCD_L_VALUE_X, LCD_L_VALUE_Y);
    lcd.printf("%5d", volumeLiterInt);
    if (first) {
      lcd.print("l");
    }
    prevVolumeLiterInt = volumeLiterInt;
  }
  lcd.setCursor(LCD_HEARTBEAT_X, LCD_HEARTBEAT_Y);
  lcd.write(heartBeat[iHeartBeat]);
  iHeartBeat = (iHeartBeat + 1) % (sizeof(heartBeat) / sizeof(heartBeat[0]));
  first = false;
}

/*!
 * Publish the measurements to MQTT
 *
 * Publishing frequency is restricted.
 *
 * @param tDegreesCelcius temperature in degrees Celcius
 * @param volumeLiter volume in litres
 * @param volumePercent volume as a percentage
 * @param lowBool low indicator
 */
void publishOnMqtt(double tDegreesCelcius, double volumeLiter, double volumePercent, bool lowBool) {
  static unsigned long tPrevWriting;
  static bool first = true;
  unsigned long tNowWriting;

  tNowWriting = millis();
  if (first || (tNowWriting - tPrevWriting) >= MQTT_INTERVAL) {
    JSONVar myObject;
    myObject["t_C"] = tDegreesCelcius;
    myObject["vol_l"] = volumeLiter;
    myObject["vol_percent"] = volumePercent;
    myObject["low"] = lowBool;
    Serial.print("myObject.keys() = ");
    Serial.println(myObject.keys());
    String jsonString = JSON.stringify(myObject);
    if (!mqttClient.publish("garden/waterlevel", jsonString)) {
#if DEBUG_MQTT == 1
      Serial.println("MQTT publishing failed.");
#endif
      return;
    }

    // If we get here, the write was successful
    lcd.setCursor(LCD_HEARTBEAT_X, LCD_HEARTBEAT_Y);
    lcd.write('.');
    
    tPrevWriting = tNowWriting;
    first = false;
  }
}

/*!
 * Check buttons and do appropriate actions
 */
void handleButtons() {
  static unsigned long tPrevBacklight = 0;
  unsigned long tNow;
  bool pushed;

  tNow = millis();
  // control LCD backlight
  if (tNow - tPrevBacklight >= BUTTON_DEBOUNCE_DELAY) {
    pushed = (digitalRead(BACKLIGHT_BUTTON_IOPIN) == 0);
    if (pushed != backlightOn) {
      if (pushed) {
        lcd.backlight();
      } else {
        lcd.noBacklight();
      }
      tPrevBacklight = tNow;
      backlightOn = pushed;
    }
  }
}
