/*!
 * @file RecieverLCDMQTT.ino
 *
 * Read out the WaterLevelTransmitter connected on Serial2 and publish the values on:
 * - a character-based LCD display using the hd44780 chip
 * - MQTT, to a topic defined in STATE_TOPIC; note that Home Assistant discovery is supported
 *
 * Tested on an ESP32 DevKitC (Espressif).
 *
 * Previous tested version:
 * - Build on Arduino IDE 2.3.3
 * - Using esp32 by Espressif Systems board manager 3.0.7, selected board: ESP32 Dev Module
 * - With libraries (copied from verbose compile output)
 *   Using library Arduino_JSON at version 0.2.0 in folder: C:\Users\marti\Documents\Arduino\libraries\Arduino_JSON 
 *   Using library Wire at version 3.0.7 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.0.7\libraries\Wire 
 *   Using library hd44780 at version 1.3.2 in folder: C:\Users\marti\Documents\Arduino\libraries\hd44780 
 *   Using library EspMQTTClient at version 1.13.3 in folder: C:\Users\marti\Documents\Arduino\libraries\EspMQTTClient 
 *   Using library ArduinoOTA at version 3.0.7 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.0.7\libraries\ArduinoOTA 
 *   Using library Networking at version 3.0.7 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.0.7\libraries\Network 
 *   Using library Update at version 3.0.7 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.0.7\libraries\Update 
 *   Using library PubSubClient at version 2.8 in folder: C:\Users\marti\Documents\Arduino\libraries\PubSubClient 
 *   Using library WiFi at version 3.0.7 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.0.7\libraries\WiFi 
 *   Using library WebServer at version 3.0.7 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.0.7\libraries\WebServer 
 *   Using library FS at version 3.0.7 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.0.7\libraries\FS 
 *   Using library ESPmDNS at version 3.0.7 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.0.7\libraries\ESPmDNS 
 *
 * About library EspMQTTClient latest version (1.13.3) by Patrick Lapointe:
 *   It contains a compatibility issue w.r.t. Espressif Systems board manager 3.x
 *   Solution according to https://forum.arduino.cc/t/broken-dependencies/1266624/10 is working:
 *     Open c:\Users\<you>\Documents\Arduino\libraries\EspMQTTClient\src\EspMQTTClient.h
 *     Change this:
 *       #else // for ESP32
 *         #include <WiFiClient.h>
 *     into this:
 *       #else // for ESP32
 *         #include <WiFi.h>
 *         #include <WiFiClient.h>
 *
 * Latest tested version:
 * - Build on Arduino IDE 2.3.10
 * - Using esp32 by Espressif Systems board manager 3.3.10, selected board: ESP32 Dev Module
 * - With libraries (copied from verbose compile output)
 *   Using library ArduinoJson at version 7.4.3 in folder: C:\Users\marti\Documents\Arduino\libraries\ArduinoJson 
 *   Using library Wire at version 3.3.10 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.10\libraries\Wire 
 *   Using library hd44780 at version 1.3.2 in folder: C:\Users\marti\Documents\Arduino\libraries\hd44780 
 *   Using library EspMQTTClient at version 1.13.3 in folder: C:\Users\marti\Documents\Arduino\libraries\EspMQTTClient 
 *   Using library ArduinoOTA at version 3.3.10 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.10\libraries\ArduinoOTA 
 *   Using library Networking at version 3.3.10 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.10\libraries\Network 
 *   Using library Update at version 3.3.10 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.10\libraries\Update 
 *   Using library PubSubClient at version 2.8 in folder: C:\Users\marti\Documents\Arduino\libraries\PubSubClient 
 *   Using library WiFi at version 3.3.10 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.10\libraries\WiFi 
 *   Using library WebServer at version 3.3.10 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.10\libraries\WebServer 
 *   Using library FS at version 3.3.10 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.10\libraries\FS 
 *   Using library ESPmDNS at version 3.3.10 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.10\libraries\ESPmDNS 
 *   Using library Hash at version 3.3.10 in folder: C:\Users\marti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.10\libraries\Hash  *
 * @author Martin Vanbrabant
 */

// Create your own secrets.h (mine is not in the repository), based on secrets_example.h
#include "secrets.h"

#include <ArduinoJson.h> // see https://arduinojson.org/v7/. This is not <Arduino_JSON.h>

#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
#include "LCDbarGraph.h"

#include <EspMQTTClient.h>

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

// The measurements to publish
double measuredTDegreesCelcius;   // temperature in degrees Celcius
double measuredTDegreesCelcius1;  // temperature in degrees Celcius, rounded to one digit after the decimal point
double measuredVolumeLiter;       // volume in litres
int    measuredVolumeLiterInt;    // volume in litres, integer
double measuredVolumePercent;     // volume as a percentage
int    measuredVolumePercentInt;  // volume as a percentage, integer
bool   measuredLowBool;           // low indicator

// MQTT configuration
constexpr uint16_t MY_MQTT_MAX_PACKET_SIZE = 1024;
constexpr char DEVICE_ID[] = "regenput_monitor";
constexpr char DEVICE_NAME[] = "Regenput";
constexpr char MANUFACTURER[] = "Martin Vanbrabant";
constexpr char MODEL[] = "ESP32 Regenputmeter";
constexpr char STATE_TOPIC[] = "home/regenput/state";
constexpr char AVAILABILITY_TOPIC[] = "home/regenput/availability";

EspMQTTClient mqttClient(
  SECRET_SSID,
  SECRET_WIFI_PASS,
  SECRET_MQTT_SERVER,     // MQTT Broker server ip
  SECRET_MQTT_USER,       // Can be omitted if not needed
  SECRET_MQTT_PASS,       // Can be omitted if not needed
  DEVICE_ID,              // Client name that uniquely identifies your device
  SECRET_MQTT_PORT        // The MQTT port, defaults to 1883
);

bool connected = false;

/*!
 * Preset a JSON document with the common properties for Home Assistant discovery
 */
void presetDiscovery(JsonDocument& doc)
{
  doc.clear();

  doc["availability_topic"] = AVAILABILITY_TOPIC;
  doc["payload_available"] = "online";
  doc["payload_not_available"] = "offline";

  JsonObject device = doc["device"].to<JsonObject>();
  device["name"] = DEVICE_NAME;
  device["manufacturer"] = MANUFACTURER;
  device["model"] = MODEL;
  JsonArray ids = device["identifiers"].to<JsonArray>();
  ids.add(DEVICE_ID);
}

/*!
 * Publish Home Assistant discovery for a component
 */
void publishComponentDiscovery(const char* component, const char* objectId, JsonDocument& doc)
{
  String payload;
  serializeJson(doc, payload);
  String topic = String("homeassistant/") + component + "/" + objectId + "/config";
#if DEBUG == 1
  Serial.print("Publishing discovery to topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  Serial.println(payload);
#endif

  if (!mqttClient.publish(topic.c_str(), payload.c_str(), true /* retain */)) {
#if DEBUG_MQTT == 1
    Serial.print("MQTT publishing failed for topic: ");
    Serial.println(topic);
#endif
    return;
  }
}

/*!
 * Publish all discovery configuration messages for Home Assistant discovery
 */
void publishDiscovery()
{
  JsonDocument doc;

  presetDiscovery(doc);
  doc["name"] = "Temperatuur";
  doc["unique_id"] = "regenput_temperatuur";
  doc["state_topic"] = STATE_TOPIC;
  doc["unit_of_measurement"] = "°C";
  doc["device_class"] = "temperature";
  doc["state_class"] = "measurement";
  doc["value_template"] = "{{ value_json.t_C }}";
  publishComponentDiscovery("sensor", "regenput_temperatuur", doc);

  presetDiscovery(doc);
  doc["name"] = "Volume";
  doc["unique_id"] = "regenput_volume";
  doc["state_topic"] = STATE_TOPIC;
  doc["unit_of_measurement"] = "liter";
  doc["state_class"] = "measurement";
  doc["value_template"] = "{{ value_json.vol_l }}";
  publishComponentDiscovery("sensor", "regenput_volume", doc);

  presetDiscovery(doc);
  doc["name"] = "Volume percent";
  doc["unique_id"] = "regenput_volume_percent";
  doc["state_topic"] = STATE_TOPIC;
  doc["unit_of_measurement"] = "%";
  doc["state_class"] = "measurement";
  doc["value_template"] = "{{ value_json.vol_percent }}";
  publishComponentDiscovery("sensor", "regenput_volume_percent", doc);

  presetDiscovery(doc);
  doc["name"] = "Volumecheck";
  doc["unique_id"] = "regenput_volume_laag";
  doc["state_topic"] = STATE_TOPIC;
  doc["payload_on"] = true;
  doc["payload_off"] = false;
  doc["value_template"] = "{{ value_json.low }}";
  doc["device_class"] = "problem";
  publishComponentDiscovery("binary_sensor", "regenput_volume_laag", doc);
}

/*!
 * This function is called once everything is connected (Wifi and MQTT)
 * WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
 */
void onConnectionEstablished()
{
#if DEBUG_MQTT == 1
  Serial.println("MQTT connected.");
#endif
  publishDiscovery();
  if (!mqttClient.publish(AVAILABILITY_TOPIC, "online", true)) {
#if DEBUG_MQTT == 1
    Serial.print("MQTT publishing failed for topic: ");
    Serial.println(AVAILABILITY_TOPIC);
#endif
    return;
  }
  connected = true;
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

  mqttClient.setMaxPacketSize(MY_MQTT_MAX_PACKET_SIZE);

#if DEBUG_MQTT == 1
  mqttClient.enableDebuggingMessages(); // Enable debugging messages sent to serial output
#endif

  mqttClient.enableLastWillMessage(AVAILABILITY_TOPIC, "offline", true);
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
  static double smoothedLow;

#if DEBUG == 1
  Serial.println(millis());
  Serial.println(inputString);
#endif

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, inputString);
  if (error) {
#if DEBUG == 1
    Serial.println("Unexpected input skipped.");
#endif
    return;
  }

  double tDegreesCelcius = doc["t_C"];
  double volumeLiter = doc["vol_l"];
  double volumePercent = doc["vol_percent"];
  bool low = doc["low"];
  smooth(measuredTDegreesCelcius, tDegreesCelcius, first);
  smooth(measuredVolumeLiter,     volumeLiter,     first);
  smooth(measuredVolumePercent,   volumePercent,   first);
  smooth(smoothedLow,             (double)low,     first);
  first = false;
  measuredTDegreesCelcius1 = round(measuredTDegreesCelcius * 10.0) / 10.0;
  measuredVolumeLiterInt = measuredVolumeLiter + 0.5;
  measuredVolumePercentInt = measuredVolumePercent + 0.5;
  measuredLowBool = (smoothedLow >= 0.5);

  publishOnLCD();
  publishOnMqtt();
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
 * Publish the measurements to MQTT
 *
 * Publishing frequency is restricted.
 *
 */
void publishOnMqtt() {
  static unsigned long tPrevWriting;
  static bool first = true;
  unsigned long tNowWriting;

  if (!connected) {
#if DEBUG_MQTT == 1
    Serial.println("MQTT not yet connected.");
#endif
    return;
  }
  tNowWriting = millis();
  if (first || (tNowWriting - tPrevWriting) >= MQTT_INTERVAL) {
    JsonDocument doc;
    doc["t_C"] = measuredTDegreesCelcius1;
    doc["vol_l"] = measuredVolumeLiterInt;
    doc["vol_percent"] = measuredVolumePercentInt;
    doc["low"] = measuredLowBool;
    String jsonString;
    serializeJson(doc, jsonString);
#if DEBUG == 1
    Serial.print("Publishing state to topic: ");
    Serial.println(STATE_TOPIC);
    Serial.print("Payload: ");
    Serial.println(jsonString);
#endif
    if (!mqttClient.publish(STATE_TOPIC, jsonString)) {
#if DEBUG_MQTT == 1
      Serial.print("MQTT publishing failed for topic: ");
      Serial.println(STATE_TOPIC);
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
