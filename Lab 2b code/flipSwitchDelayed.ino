const byte LEFT_INVERTER = 3;
const byte RIGHT_INVERTER = 2;
const byte SWITCH = A0;
const byte LIMIT = A1;
unsigned long prevMillis;
unsigned long currentMillis;
unsigned long passedTime;

void setup() {
  pinMode(LEFT_INVERTER, OUTPUT);
  pinMode(RIGHT_INVERTER, OUTPUT);
  digitalWrite(LEFT_INVERTER, LOW);
  digitalWrite(RIGHT_INVERTER, LOW);
  pinMode(SWITCH, INPUT_PULLUP);
  pinMode(LIMIT, INPUT_PULLUP);
  passedTime = 0;
  prevMillis = 0;
  currentMillis = 0;
}

void loop() {
  currentMillis = millis();
  if (digitalRead(SWITCH) == HIGH) {
    passedTime = passedTime + currentMillis - prevMillis;
    if (passedTime >= 1000) { // when switch was flipped & wasn't flipped back for 1 sec
      digitalWrite(LEFT_INVERTER, LOW);
      digitalWrite(RIGHT_INVERTER, HIGH);
      passedTime = 0;
    }
  } else if (digitalRead(LIMIT) == LOW) {
    digitalWrite(LEFT_INVERTER, HIGH);
    digitalWrite(RIGHT_INVERTER, LOW);
  } else {
    digitalWrite(LEFT_INVERTER, LOW);
    digitalWrite(RIGHT_INVERTER, LOW);
  }
  prevMillis = currentMillis;
}

