/*
 * Li-Fi Text Receiver (ESP8266 + solar panel)  -  REFERENCE CODE
 * --------------------------------------------------------------
 * NOTE: Reference implementation written to match the project design.
 *
 * The solar panel voltage is read on A0 (keep it below 1V on a bare
 * ESP8266 ADC pin; use a resistor divider if the panel gives more).
 * Light ON  -> higher reading, light OFF -> lower reading.
 *
 * Decoding matches the transmitter:
 *   idle = ON, start bit = OFF, 8 data bits (LSB first), stop bit = ON.
 * Decoded characters are printed to the Serial Monitor of Laptop 2.
 *
 * Tune THRESHOLD: open the Serial Monitor, set CALIBRATE to true,
 * and note the readings with the LED on and off. Use a value in
 * the middle.
 */

const int  SENSOR_PIN = A0;
const int  THRESHOLD  = 300;          // adjust after calibration
const unsigned long BIT_US = 10000;   // must match the transmitter
const bool CALIBRATE  = false;        // true = print raw readings only

inline bool lightOn() {
  return analogRead(SENSOR_PIN) > THRESHOLD;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Li-Fi receiver ready");
}

void loop() {
  if (CALIBRATE) {
    Serial.println(analogRead(SENSOR_PIN));
    delay(200);
    return;
  }

  // Wait for the start bit (light goes OFF)
  if (lightOn()) return;

  // Move to the middle of the first data bit
  unsigned long t = micros();
  while (micros() - t < BIT_US + BIT_US / 2) { yield(); }

  uint8_t c = 0;
  for (int i = 0; i < 8; i++) {
    if (lightOn()) c |= (1 << i);
    t = micros();
    while (micros() - t < BIT_US) { yield(); }
  }

  Serial.write(c);   // show the received character

  // Wait for the line to go back to idle (stop bit)
  unsigned long start = millis();
  while (!lightOn() && millis() - start < 100) { yield(); }
}
