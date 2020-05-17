/*!
 * @file WaterLevelReceiver.ino
 *
 * Read out the WaterLevelTransmitter connected on Serial2 and publish the values.
 *
 * Tested on an ESP32 DevKitC (Espressif).
 *
 * @author Martin Vanbrabant
 */

unsigned long t0;
unsigned long t1;
int i;
#define l 256
char buf[l];

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  // be patient with serial port 2:
  // wait until initialised and then flush rubbish input
  delay(1000);
  while (Serial2.available() > 0 ) {
    Serial2.read();
  }

  t0 = millis();
  i = 0;
}

bool timeToTrigger() {
  t1 = millis();
  return (t1 - t0 >= 1000ul);
}

void trigger() {
    Serial2.print("?");
    t0 = t1;
    i = 0;
}

void collect() {
  if (Serial2.available() > 0) {
    char c;
    c = Serial2.read();
    if (i == 0) {
      if (c == '{') {
        buf[i++] = c;
      }
    } else if (i < l - 1) {
      buf[i++] = c;
      if (c == '}') {
        buf[i] = '\0';
        consume();
        i = 0;
      }
    }
  }  
}

void consume() {
  // debug output
  Serial.println(t1);
  Serial.println(buf);
}

void loop() {
  if (timeToTrigger()) {
    trigger();
  } else {
    collect();
  }
}
