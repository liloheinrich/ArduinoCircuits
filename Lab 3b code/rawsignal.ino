/* LED plane responding to audio by amplitude (starter code)
 *
 * Lights an LED array to respond to audio.
 *
 * == Setting up the Serial Monitor ==
 * The Serial Monitor must be configured (bottom-right corner of the screen) as
 * baud rate 115200.
 *
 * ENGR 40M
 * Stanford University
 * July 2018
 */

const byte DIM = 8;
const byte ANODE_PINS[DIM] = {13, 12, 11, 10, 9, 8, 7, 6};
const byte CATHODE_PINS[DIM] = {A3, A2, A1, A0, 2, 3, 4, 5};
const byte AUDIO_INPUT_PIN = A5;
unsigned long currentMillis;
unsigned long startMillis;
long sampleSum;
unsigned int numSamples;

void setup() {
  for(byte anode : ANODE_PINS){
    pinMode(anode, OUTPUT);
    digitalWrite(anode, HIGH);
  }
  for(byte cathode : CATHODE_PINS){
    pinMode(cathode, OUTPUT);
    digitalWrite(cathode, HIGH);
  }
  pinMode(AUDIO_INPUT_PIN, INPUT);
  Serial.begin(115200);
  startMillis = millis();
  currentMillis = startMillis;
}

/* Function: display
 * -----------------
 * Runs through one multiplexing cycle of the LEDs, controlling which LEDs are
 * on.
 *
 * During this function, LEDs that should be on will be turned on momentarily,
 * one row at a time. When this function returns, all the LEDs will be off
 * again, so it needs to be called continuously for LEDs to be on.
 */
void display(byte pattern[8][8]) {
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
      delayMicroseconds((b+1)*(b+1)); 
//      ^^ use log(2)N scale for delay to make brightness more contrasting to human eye
//      delayMicroseconds(1);
    }
    digitalWrite(ANODE_PINS[r], HIGH);
  }
}

/* Function: getSample
 * -------------------
 * Returns an unbiased sample of the audio signal. The returned value will be
 * "centered around zero", i.e., sometimes positive and sometimes negative.
 */
int getSample() {
  int sample = abs(analogRead(AUDIO_INPUT_PIN)-512);
  // You might want to do some processing here, to unbias the sample.
  Serial.println(sample); // to serial monitor
  return sample;
}


/* Function: setLEDs
 * -----------------
 * Sets the LEDs array based on the given sample. The sample is assumed to
 * be returned by getSample() (and have the consequent properties).
 */
void setLEDs(byte pattern[8][8], int sample) {
  static byte range = 30; // experimentally found that this range was ok for music volume level on my computer
  static byte timeWaitMillis = 1000;
  currentMillis = millis();
  if(currentMillis - startMillis >= timeWaitMillis){
    for(int i = 0; i < 7; i++){ // shift cols
      for(int j = 0; j < 8; j++){ 
        pattern[j][i] = pattern[j][i+1];
      }
    }
    for (int j = 0; j < 8; j++) { // write new col
      if (8 - j <= (sampleSum/numSamples)*8 / (range)) {
        pattern[j][7] = (sampleSum/numSamples)*16 / (range); // make brightness dependent on sample volume
//        pattern[j][7] = 1; // make brightness not variable
      }else{
        pattern[j][7] = 0;
      }
    }
    startMillis = currentMillis; // reset timer to wait again
    sampleSum = sample; // reset samples info so it doesn't accumulate
    numSamples = 1;
  }else{
    sampleSum += sample; // collect data while waiting
    numSamples++;
  }
}

void loop() {
  static byte pattern[8][8];
  int sample = getSample();
  setLEDs(pattern, sample);
  display(pattern);
}
