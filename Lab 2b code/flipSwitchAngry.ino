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
  if (digitalRead(SWITCH) == HIGH) { // swtich flipped
    passedTime = passedTime + currentMillis - prevMillis;
    if (passedTime >= 350 && passedTime < 550) { // motor reverse
      digitalWrite(LEFT_INVERTER, HIGH);
      digitalWrite(RIGHT_INVERTER, LOW);
    }else if(passedTime < 350){ // motor forward
      digitalWrite(LEFT_INVERTER, LOW);
      digitalWrite(RIGHT_INVERTER, HIGH);
    }else if(passedTime >= 550){ // set motor and reset timer
      digitalWrite(LEFT_INVERTER, LOW);
      digitalWrite(RIGHT_INVERTER, LOW);
      passedTime = 0;
    }
  } else if (digitalRead(LIMIT) == LOW) { // limit switch unpressed
    digitalWrite(LEFT_INVERTER, HIGH);
    digitalWrite(RIGHT_INVERTER, LOW);
    passedTime = 0;
  } else { // stop motor
    digitalWrite(LEFT_INVERTER, LOW);
    digitalWrite(RIGHT_INVERTER, LOW);
    passedTime = 0;
  }
  prevMillis = currentMillis;
}

