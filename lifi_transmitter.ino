/*
 * Li-Fi Text Transmitter (ESP32 + LED)  -  REFERENCE CODE
 * -------------------------------------------------------
 * NOTE: Reference implementation written to match the project design.
 *
 * The laptop sends characters over USB serial (see sender.cpp).
 * The ESP32 sends each character as light using simple serial-style
 * framing with an LED:
 *   idle      = LED ON
 *   start bit = LED OFF
 *   8 data bits, LSB first (1 = LED ON, 0 = LED OFF)
 *   stop bit  = LED ON
 * After each character the ESP32 sends '.' back to the laptop as an
 * acknowledgement, so the laptop never overflows the serial buffer.
 *
 * Keep BIT_US the same in the receiver.
 */

const int LED_PIN = 25;
const unsigned long BIT_US = 10000;   // 10 ms per bit (about 100 bits/second)

inline void sendBit(bool b) {
  digitalWrite(LED_PIN, b ? HIGH : LOW);
  delayMicroseconds(BIT_US);
}

void sendByte(uint8_t c) {
  sendBit(0);                              // start bit
  for (int i = 0; i < 8; i++) {
    sendBit((c >> i) & 0x01);              // data bits, LSB first
  }
  sendBit(1);                              // stop bit
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);             // idle = ON
  delay(500);
}

void loop() {
  if (Serial.available()) {
    uint8_t c = Serial.read();
    sendByte(c);
    Serial.write('.');                     // acknowledgement to laptop
  }
}
