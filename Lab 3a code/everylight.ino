const byte ANODE_PINS[8] = {13, 12, 11, 10, 9, 8, 7, 6};
const byte CATHODE_PINS[8] = {A3, A2, A1, A0, 2, 3, 4, 5};

void setup() {
  for(byte anode : ANODE_PINS){
    pinMode(anode, OUTPUT);
    digitalWrite(anode, HIGH);
  }
  for(byte cathode : CATHODE_PINS){
    pinMode(cathode, OUTPUT);
    digitalWrite(cathode, HIGH);
  }
}

void loop(){
  for(byte anode : ANODE_PINS){
    digitalWrite(anode, LOW);
    for(byte cathode : CATHODE_PINS){
      digitalWrite(cathode, LOW);
      delay(100); // ms to delay
      digitalWrite(cathode, HIGH);
    }
    digitalWrite(anode, HIGH);
  }
}
