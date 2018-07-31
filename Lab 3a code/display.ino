const byte DIM = 8;
const byte ANODE_PINS[DIM] = {13, 12, 11, 10, 9, 8, 7, 6};
const byte CATHODE_PINS[DIM] = {A3, A2, A1, A0, 2, 3, 4, 5};

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
  static byte pattern[DIM][DIM];
  pattern[0][0] = 1;
  pattern[0][7] = 2;
  pattern[7][7] = 3;
  pattern[7][0] = 4;
  display(pattern);
}

void display(byte pattern[DIM][DIM]){
  for(byte r = 0; r < 8; r++){
    for(byte c = 0; c < 8; c++){
      if(pattern[r][c] > 0){
        digitalWrite(CATHODE_PINS[c], LOW);
      }else{
        digitalWrite(CATHODE_PINS[c], HIGH);
      }
    }
    digitalWrite(ANODE_PINS[r], LOW);
    delay(1);
    digitalWrite(ANODE_PINS[r], HIGH);
  }
}

