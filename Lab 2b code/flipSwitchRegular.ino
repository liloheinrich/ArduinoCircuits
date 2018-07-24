const byte LEFT_INVERTER = 3;
const byte RIGHT_INVERTER = 2;
const byte SWITCH = A0;
const byte LIMIT = A1;

void setup() {
  pinMode(LEFT_INVERTER, OUTPUT);
  pinMode(RIGHT_INVERTER, OUTPUT);
  digitalWrite(LEFT_INVERTER, LOW);
  digitalWrite(RIGHT_INVERTER, LOW);
  pinMode(SWITCH, INPUT_PULLUP);
  pinMode(LIMIT, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(SWITCH) == HIGH) {
    digitalWrite(LEFT_INVERTER, LOW);
    digitalWrite(RIGHT_INVERTER, HIGH);
  } else if (digitalRead(LIMIT) == LOW) {
    digitalWrite(LEFT_INVERTER, HIGH);
    digitalWrite(RIGHT_INVERTER, LOW);
  } else {
    digitalWrite(LEFT_INVERTER, LOW);
    digitalWrite(RIGHT_INVERTER, LOW);
  }
}

