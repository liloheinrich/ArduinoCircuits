/* LED array responding to audio by frequency (starter code)

   Lights an LED array to respond to the frequency spectrum of an audio signal.

   == Setting up the Serial Monitor ==
   The Serial Monitor must be configured (bottom-right corner of the screen) as
   baud rate 115200.

   ENGR 40M
   Stanford University
   July 2018
*/

// FFT library options
// See http://wiki.openmusiclabs.com/wiki/Defines if you're curious.
#define FFT_N 16 // you can change this if you like - see the note in setLEDs.
// (you might consider changing it to 64)
#define WINDOW 0
#define OCTAVE 1
#define OCT_NORM 0
#include <FFT.h>

const byte SAMPLE_PERIOD_MICROSECONDS = 250;
const byte DIM = 8;
const byte ANODE_PINS[DIM] = {13, 12, 11, 10, 9, 8, 7, 6};
const byte CATHODE_PINS[DIM] = {A3, A2, A1, A0, 2, 3, 4, 5};
const byte AUDIO_INPUT_PIN = A5;

void setup() {
  for (byte anode : ANODE_PINS) {
    pinMode(anode, OUTPUT);
    digitalWrite(anode, HIGH);
  }
  for (byte cathode : CATHODE_PINS) {
    pinMode(cathode, OUTPUT);
    digitalWrite(cathode, HIGH);
  }
  pinMode(AUDIO_INPUT_PIN, INPUT);
  Serial.begin(115200);
}

/* Function: display
   -----------------
   Runs through one multiplexing cycle of the LEDs, controlling which LEDs are
   on.

   During this function, LEDs that should be on will be turned on momentarily,
   one row at a time. When this function returns, all the LEDs will be off
   again, so it needs to be called continuously for LEDs to be on.
*/
void display(byte pattern[8][8]) {
  for (byte r = 0; r < 8; r++) {
    digitalWrite(ANODE_PINS[r], LOW);
    for (byte b = 0; b < 16; b++) {
      for (byte c = 0; c < 8; c++) {
        if (pattern[r][c] > b) {
          digitalWrite(CATHODE_PINS[c], LOW);
        } else {
          digitalWrite(CATHODE_PINS[c], HIGH);
        }
      }
      delayMicroseconds((b + 1) * (b + 1));
      //      ^^ use log(2)N scale for delay to make brightness more contrasting to human eye
//            delayMicroseconds(1);
    }
    digitalWrite(ANODE_PINS[r], HIGH);
  }
}

/* Function: getSamples
   --------------------
   Populates every other element of an array with FFT_N samples, sampled at
   1000/SAMPLE_PERIOD_MICROSECONDS kHz. Takes about 6.7ms to run.
*/
void getSamples(int samples[FFT_N * 2]) {
  unsigned long next_sample_time = micros();
  for (byte i = 0; i < FFT_N * 2; i += 2) {
    while (micros() < next_sample_time); // wait till next sample time
    unsigned long value = analogRead(AUDIO_INPUT_PIN);
    samples[i] = value - 512;
    samples[i + 1] = 0;
    next_sample_time += SAMPLE_PERIOD_MICROSECONDS;
  }
} 

/* Function: printBins
   -------------------
   Prints the contents of the fft_oct_out bins to the Serial Monitor.
   Assumes that LOG_N == 4; change this is that's not the case.
*/
void printBins(byte bins[LOG_N]) {
  static char message[20];
  sprintf(message, "%3d %3d %3d %3d\n", fft_oct_out[0], fft_oct_out[1], fft_oct_out[2], fft_oct_out[3]);
  Serial.print(message);
}

/* Function: setLEDs
   -----------------
   Sets the LEDs array based on the given frequency bin values. The bins are
   assumed to be of the form generated by fft_mag_octave() of the ArduinoFFT
   library.

   LOG_N is defined in FFT.h as log2(FFT_N), so if FFT_N = 16, LOG_N = 4.
*/
void setLEDs(byte pattern[8][8], byte bins[LOG_N]) {
  static byte minLoudness = 30;
  static byte clefBrightness[4] = {10, 2, 15, 6}; // makes diff cols different colors kind of for contrast to the eye
  static byte range = 100 - minLoudness; // max val I found in bins was in the 90's so i set max to 100
  for (int k = 0; k < 4; k++) { // clefs
    for (int i = 0; i < 2; i++) { // cathodes for a clef
      for (int j = 0; j < 8; j++) { // anodes
        if (8 - j <= (bins[k] - minLoudness) / ((range - minLoudness) / 8)) {
          pattern[j][i + 2 * k] = clefBrightness[k];
        } else {
          pattern[j][i + 2 * k] = 0;
        }
      }
    }
  }
}

void loop() {
  static byte pattern[8][8];
  getSamples(fft_input);

  // These three functions run the FFT on the data in the global array
  // fft_input. The result will be in the global array fft_oct_out. If you're
  // curious, you can look at http://wiki.openmusiclabs.com/wiki/FFTFunctions
  // for other variants of these functions to use instead.
  fft_reorder();
  fft_run();
  fft_mag_octave();

  printBins(fft_oct_out);
  setLEDs(pattern, fft_oct_out);
  display(pattern);
}

