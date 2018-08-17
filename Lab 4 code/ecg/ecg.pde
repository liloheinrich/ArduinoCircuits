
/* Plot values received from serial port.
 *
 * Processing code to take ASCII-encoded strings from the serial port at 115200
 * baud and graph them. It expects values in the range 0 to 1023, followed by a
 * newline.
 *
 * Based on public domain code available at http://arduino.cc/en/Tutorial/Graph
 *
 * ENGR 40M
 * Stanford University
 * August 2018
 */

import processing.serial.*;
import java.util.*;

// You can run this code with empty string "" to get list of available serial port names.
final String SERIAL_PORT_NAME = "COM5";

// color() takes three arguments, (red, green, blue), in a 0-255 range.
final color BACKGROUND_COLOR = color(0, 0, 0);    // black (feel free to change)
final color FOREGROUND_COLOR = color(0, 255, 0);  // green (feel free to change)
final int K = 50; // number of samples to take and average out. seems like a high number but it worked.

Serial myPort;   // Serial port object  
int x = 1;       // x-coordinate
float y = 0;     // y-coordinate
float lastY;     // old y-coordinate
ArrayList<Float> pastVals;

void setup () {
  // Set the window size
  // TODO: Adjust this to fit your computer screen
  pastVals = new ArrayList<Float>();
  size(1200, 800);

  if (SERIAL_PORT_NAME == "") {
    println("Here is a list of serial port names:");
    for (String name : Serial.list()) println(name);
    println("Please set SERIAL_PORT_NAME to one of these names.");
    exit();
    return;
  }

  myPort = new Serial(this, SERIAL_PORT_NAME, 115200);
  myPort.bufferUntil('\n');      // trigger serialEvent() only on newline
  background(BACKGROUND_COLOR);  // set background color
  noLoop();                      // draw() only when triggered by redraw()
}

/* Function: draw()
 * -----------------------
 * Draws to the screen. Normally loops continuously, but if noLoop() is called
 * in setup(), then this is triggered only when redraw() is called by another
 * function (in our case, by serialEvent()).
 */
void draw () {
  stroke(BACKGROUND_COLOR);
  line(x, height, x, 0);    // erase the next x position

  stroke(FOREGROUND_COLOR);
  line(x-1, lastY, x, y);   // line from last point to this point
  lastY = y;

  // If at the end of the screen, go back to the beginning
  if (x >= width) x = 1;   // not zero, the line starts at xPos-1
  else x = x + 2;
}

/* Function: serialEvent()
 * -----------------------
 * Runs every time a newline is received on the serial port.
 */
void serialEvent (Serial myPort) {
  String inString = myPort.readStringUntil('\n');
  if (inString == null) return;

  inString = trim(inString);       // remove leading/trailing whitespace
  float inByte = float(inString);  // convert to float
  float output = filter(inByte);   // run through "filter()" below
  if (output == -1) return;        // don't plot if error given by filter()

  // Map from a 0-to-1023 scale to a height-to-0 scale. Processing's y = 0 is at
  // the top, "upside-down" relative to graph coordinates.
  y = map(output, 0, 1024, height, 0);
  
  redraw();                        // this triggers a call of draw()
}

/* Function: filter()
 * ------------------
 * Every time this function is called, a new sample is provided, and this
 * function should return a smoothed value. If you can't return a value and want
 * to "skip" this point, return -1.
 *
 * The current implementation of this just returns the same value.
 */
float filter(float input) {
  // Do some filtering here. You'll probably need to keep track of some of
  // the old values. Processing uses Java, so you can use arrays, queues, etc.
  if(pastVals.size() >= K){
    pastVals.remove(0);
  }
  pastVals.add(input);
  float sum = 0;
  for(float f : pastVals){
    sum += f;
  }
  return sum / pastVals.size();
}
