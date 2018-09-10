// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            7

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      25


//Serial input to fake the Mqtt Connection
const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data


int recievedValue;
boolean newData = false;

int oldPos=0;
int newPos;

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 50; // delay for half a second

void setup() {
   Serial.begin(9600);
   Serial.println("<Arduino is ready>");

  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {

  recvWithEndMarker();
  convertData();




      
    }







void moveTrackMotion(int End){

        int wayLength = oldPos-End;

        for (int i = oldPos;i<End;i++)
          {
                    // wait for 30 milliseconds to see the dimming effect
          for(int i=0;i<NUMPIXELS;i++){
      
              // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
              pixels.setPixelColor(i, pixels.Color(10,10,10)); // Moderately bright green color.
          
              pixels.show(); // This sends the updated pixel color to the hardware
          }
          
          pixels.setPixelColor(i, pixels.Color(10,150,10)); // Moderately bright green color.
      
          pixels.show(); // This sends the updated pixel color to the hardware.
              
          }
    oldPos= End;
  }



void recvWithEndMarker() {
 static byte ndx = 0;
 char endMarker = '\n';
 char rc;
 
 // if (Serial.available() > 0) {
 while (Serial.available() > 0 && newData == false) {
 rc = Serial.read();

 if (rc != endMarker) {
     receivedChars[ndx] = rc;
     ndx++;
     
       if (ndx >= numChars) {
         ndx = numChars - 1;
       }
 }
 else {
     receivedChars[ndx] = '\0'; // terminate the string
     ndx = 0;
     newData = true;
      }
 }
}

void convertData() {
 if (newData == true) {
  String command = String(receivedChars);
  newPos = command.toInt();
  Serial.println(newPos);
  newData = false;
  moveTrackMotion(newPos);
 }
}
