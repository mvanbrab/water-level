/*!
 * @file ReceiverSerial.ino
 *
 * Read out the WaterLevelTransmitter connected on Serial2 and publish the values on:
 * - the serial port accessible through the USB connection the serial line.
 * 
 * Tested on an ESP32 DevKitC (Espressif).
 *
 * Last tested version:
 * - Build on Arduino IDE 2.3.3
 * - Using esp32 by Espressif Systems board manager 3.0.7, selected board: ESP32 Dev Module
 * - With libraries (copied from verbose compile output)
 *   Using library Arduino_JSON at version 0.2.0 in folder: C:\Users\marti\Documents\Arduino\libraries\Arduino_JSON 
 *
 * @author Martin Vanbrabant
 */

#include <Arduino_JSON.h>

// 1: debug output on serial, 0: no debug output
#define DEBUG 0
// 1: start by processing some fake input, 0: no fake input
#define DEBUG_FAKE_INPUT 0

// IO pins
#define BACKLIGHT_BUTTON_IOPIN 12

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

/*!
 * Arduino's setup function
 */
void setup() {
  pinMode(BACKLIGHT_BUTTON_IOPIN, INPUT_PULLUP);

  Serial.begin(115200);
  Serial2.setPins(16, 17); // used to be default, now needed for board manager v3, see https://docs.espressif.com/projects/arduino-esp32/en/latest/migration_guides/2.x_to_3.0.html
  Serial2.begin(115200);
  unsigned long tSerial2 = millis();

  // be patient with serial port 2:
  // wait until initialised and then flush rubbish input
  while (millis() - tSerial2 < INITIAL_SERIAL2_DELAY) {
    // empty
  }
  while (Serial2.available() > 0 ) {
    Serial2.read();
  }
}

/*!
 * Arduino's loop function
 */
void loop() {
#if DEBUG_FAKE_INPUT == 0
  handleTransmitter();
#else
  handleFakeInput();
#endif
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

  publishOnSerial();
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
 */
void publishOnSerial() {
  char buf[100];
  sprintf(buf, "t = %lf °C", measuredTDegreesCelcius);
  Serial.println(buf);
  sprintf(buf, "volume = %lf l (%lf %%)%s", measuredVolumeLiter, measuredVolumePercent, measuredLowBool ? " LOW!" : "");
  Serial.println(buf);
}
