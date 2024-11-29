/*!
 * @file WaterLevelReceiver.ino
 *
 * Read out the WaterLevelTransmitter connected on Serial2 and publish the values on:
 * - a character-based LCD display using the hd44780 chip
 * - a locally hosted webpage
 *
 * Tested on an ESP32 DevKitC (Espressif).
 *
 * Last tested version:
 * - Build on Arduino IDE 2.3.3
 * - Using esp32 by Espressif Systems board manager 3.0.7, selected board: ESP32 Dev Module
 * - With libraries (copied from verbose compile output)
 *   Using library Arduino_JSON at version 0.2.0 in folder: C:\Users\marti\Documents\Arduino\libraries\Arduino_JSON 
 *   Using library Wire at version 3.0.7 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.0.7\libraries\Wire 
 *   Using library hd44780 at version 1.3.2 in folder: C:\Users\marti\Documents\Arduino\libraries\hd44780 
 *   Using library WiFi at version 3.0.7 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.0.7\libraries\WiFi 
 *   Using library Networking at version 3.0.7 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.0.7\libraries\Network 
 *   Using library WebServer at version 3.0.7 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.0.7\libraries\WebServer 
 *   Using library FS at version 3.0.7 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.0.7\libraries\FS 
 *
 * @author Martin Vanbrabant
 */

// Create your own secrets.h (mine is not in the repository), based on secrets_template.h
#include "secrets.h"

#include <Arduino_JSON.h>

#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
#include "LCDbarGraph.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

// 1: debug output (general) on serial, 0: no debug output (general)
#define DEBUG 0
// 1: debug output (HTTP) on serial, 0: no debug output (HTTP)
#define DEBUG_HTTP 1
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
#define BUTTON_DEBOUNCE_DELAY 100ul

// The measurements to publish
double measuredTDegreesCelcius;   // temperature in degrees Celcius
double measuredVolumeLiter;       // volume in litres
int    measuredVolumeLiterInt;    // volume in litres, integer
double measuredVolumePercent;     // volume as a percentage
int    measuredVolumePercentInt;  // volume as a percentage, integer
bool   measuredLowBool;           // low indicator

WebServer server(80);

/*!
 * Handles HTTP requests at "/"
 */
void handleRoot() {
  static char htmlRepresentation[1000];

  // for snprintf_P, see https://cpp4arduino.com/2020/02/07/how-to-format-strings-without-the-string-class.html
  snprintf_P(htmlRepresentation, sizeof(htmlRepresentation),
PSTR("<!DOCTYPE html>"
"<html>"
"<head>"
"<style>"
"table {"
"border: 1px solid black;"
"}"
"td {"
"padding-left: 10px;"
"padding-right: 50px;"
"}"
"#volm {"
"width: 100%%;"
"}"
"#vol, #volp {"
"%s"
"}"
"#temp {"
"%s"
"}"
"</style>"
"</head>"
"<body>"
"<h1>Waterlevel</h1>"
"<table>"
"<tr>"
"<td><h2>Volume</h2></td>"
"<td><h2 id=\"vol\">%d l</h2></td>"
"</tr>"
"<tr>"
"<td><meter id=\"volm\" value=\"%d\" min=\"0\" low=\"%d\" max=\"100\"></meter></td>"
"<td><h2 id=\"volp\">%d &percnt;</h2></td>"
"</tr>"
"<tr>"
"<td><h2>Temperature</h2></td>"
"<td><h2 id=\"temp\">%.1lf &deg;C</h2></td>"
"</tr>"
"</table>"
"</body>"
"</html>"),
  (measuredLowBool ? "color: red;" : ""),
  (measuredTDegreesCelcius < 0.0 ? "color: red;" : ""),
  measuredVolumeLiterInt,
  measuredVolumePercentInt,
  (measuredLowBool ? measuredVolumePercentInt + 1 : 0),
  measuredVolumePercentInt,
  measuredTDegreesCelcius
  );
#if DEBUG_HTTP == 1
  Serial.println(htmlRepresentation);
#endif
  server.send(200, "text/html", htmlRepresentation);
}

void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

/*!
 * Arduino's setup function
 */
void setup() {
  pinMode(BACKLIGHT_BUTTON_IOPIN, INPUT_PULLUP);

  Serial.begin(115200);
  Serial2.setPins(16, 17); // used to be default, now needed for board manager v3, see https://docs.espressif.com/projects/arduino-esp32/en/latest/migration_guides/2.x_to_3.0.html
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

#if DEBUG_HTTP == 1
  Serial.print("Connecting to ");
  Serial.println(SECRET_SSID);
#endif
  WiFi.setHostname(SECRET_HOSTNAME);
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(SECRET_SSID, SECRET_WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#if DEBUG_HTTP == 1
    Serial.print(".");
#endif
  }

#if DEBUG_HTTP == 1
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
}

/*!
 * Arduino's loop function
 */
void loop() {
   server.handleClient();
   delay(100);
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
  static char inputBuffer[256];
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
      } else if (iInputBuffer < sizeof(inputBuffer) - 1) {
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

  smooth(measuredTDegreesCelcius, (double)myObject["t_C"],           first);
  smooth(measuredVolumeLiter,     (double)((int) myObject["vol_l"]), first);
  smooth(measuredVolumePercent,   (double)myObject["vol_percent"],   first);
  smooth(low,                     (double)((bool) myObject["low"]),  first);
  first = false;
  measuredVolumeLiterInt = measuredVolumeLiter + 0.5;
  measuredVolumePercentInt = measuredVolumePercent + 0.5;
  measuredLowBool = (low >= 0.5);

  publishOnLCD();
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
 * Publish the measurements to the LCD display
 */
void publishOnLCD() {
  static bool first = true;
  static int prevVolumeLiterInt = -1;
  static int prevVolumePercentInt = -1;
  static bool prevLowBool = false;
  static const char heartBeat[] = {'*', ' '};
  static uint8_t iHeartBeat = 0;

  if (measuredVolumePercentInt != prevVolumePercentInt) {
    lcd.setCursor(LCD_PERCENT_VALUE_X, LCD_PERCENT_VALUE_Y);
    lcd.printf("%3d", measuredVolumePercentInt);
    if (first) {
      lcd.print("%");
    }
    barGraph.display(measuredVolumePercentInt);
    prevVolumePercentInt = measuredVolumePercentInt;
  }
  if (measuredLowBool != prevLowBool) {
    lcd.setCursor(LCD_LOW_VALUE_X, LCD_LOW_VALUE_Y);
    lcd.print(measuredLowBool ? "LOW" : "   ");
    prevLowBool = measuredLowBool;
  }
  if (measuredVolumeLiterInt != prevVolumeLiterInt) {
    lcd.setCursor(LCD_L_VALUE_X, LCD_L_VALUE_Y);
    lcd.printf("%5d", measuredVolumeLiterInt);
    if (first) {
      lcd.print("l");
    }
    prevVolumeLiterInt = measuredVolumeLiterInt;
  }
  lcd.setCursor(LCD_HEARTBEAT_X, LCD_HEARTBEAT_Y);
  lcd.write(heartBeat[iHeartBeat]);
  iHeartBeat = (iHeartBeat + 1) % (sizeof(heartBeat) / sizeof(heartBeat[0]));
  first = false;
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
