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
  pattern[0][0] = 1; // these lines are my test case
  pattern[0][1] = 3;
  pattern[0][2] = 5;
  pattern[0][3] = 7;
  pattern[0][4] = 9;
  pattern[0][5] = 11;
  pattern[0][6] = 13;
  pattern[0][7] = 15;
  display(pattern);
}

void display(byte pattern[DIM][DIM]){
  for(byte r = 0; r < 8; r++){
    digitalWrite(ANODE_PINS[r], LOW);
    for(byte b = 0; b < 16; b++){
      for(byte c = 0; c < 8; c++){
        if(pattern[r][c] > b){
          digitalWrite(CATHODE_PINS[c], LOW);
        }else{
          digitalWrite(CATHODE_PINS[c], HIGH);
        }
      }
//      delayMicroseconds((b+1)*(b+1)); 
//      ^^ use log(2)N scale for delay to make brightness more contrasting to human eye
      delayMicroseconds(1);
    }
    digitalWrite(ANODE_PINS[r], HIGH);
  }
}

