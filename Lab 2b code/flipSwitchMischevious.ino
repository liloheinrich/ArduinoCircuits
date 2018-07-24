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
  if (digitalRead(SWITCH) == HIGH) { // switch flipped
    passedTime = passedTime + currentMillis - prevMillis;
    if(passedTime < 300){ // first: go forward
      digitalWrite(LEFT_INVERTER, LOW);
      digitalWrite(RIGHT_INVERTER, HIGH);
    }else if(passedTime >= 300 && passedTime < 1300){ // second: stop for 1 sec
      digitalWrite(LEFT_INVERTER, LOW);
      digitalWrite(RIGHT_INVERTER, LOW);
    }else{ // third: reset timer and set to forward again
      digitalWrite(LEFT_INVERTER, LOW);
      digitalWrite(RIGHT_INVERTER, LOW);
      passedTime = 0;
    }
  } else if (digitalRead(LIMIT) == LOW) { // limit switch unpressed - motor reverse
    digitalWrite(LEFT_INVERTER, HIGH);
    digitalWrite(RIGHT_INVERTER, LOW);
    passedTime = 0;
  } else { // motor stop
    digitalWrite(LEFT_INVERTER, LOW);
    digitalWrite(RIGHT_INVERTER, LOW);
    passedTime = 0;
  }
  prevMillis = currentMillis;
}

