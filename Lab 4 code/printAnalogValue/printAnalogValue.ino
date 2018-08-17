byte ANALOG_PIN = A1;

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println(analogRead(ANALOG_PIN));
}
