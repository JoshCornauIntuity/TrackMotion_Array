// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPL3 license to match the rest of the AdaFruit NeoPixel lirary

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
unsigned long currentMillis;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int recievedValue;
boolean newData = false;

int pos = 1;

int newPos;

boolean moveTrack;
boolean mDirection;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 10;           // interval at which to blink (milliseconds)

void setup() {
   Serial.begin(9600);
   Serial.println("<Arduino is ready>");

   pixels.begin();
}

void loop() {
  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
 
  
  currentMillis = millis();
  recvWithEndMarker();
  giveCommand();


  //moveTheSweets when a message arrives
  if(moveTrack){
      moveTrackMotion(newPos);
    }

  
}




void moveTrackMotion(int End){  

//  Serial.print(End);
  if(pos == End){
        Serial.println("End");
              //Stops the motion
        moveTrack= false;
        
        //update the old position to the new position
        pos = End;
        

   }
   else {
    if (currentMillis - previousMillis >= interval) {
      // save the last time you moved the sweets
        previousMillis = currentMillis;
         //Clears the Stripe
         for(int i=0;i<NUMPIXELS;i++){
          
                  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
                  pixels.setPixelColor(i, pixels.Color(10,10,10)); // Moderately bright green color.
              
                 // This sends the updated pixel color to the hardware
              }   
//        Serial.println("once");
        //Depending on the Direction the pos increase or decrease
        if (!mDirection){
            pos +=1;
        }
        else{
            pos -=1;
        } 
      

        Serial.println(pos);
        Serial.println(End);
        pixels.setPixelColor(pos, pixels.Color(10,150,10));
        pixels.show(); 
      }
    }
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

//sets the moveTrack to true, converts the message from Serial, and the direction the track should move
void giveCommand() {
   if (newData == true) {
      String command = String(receivedChars);
      newPos = command.toInt();
      
      newData = false;
      moveTrack = true;
      previousMillis = currentMillis;
      //get Direction
      mDirection = getDirection(newPos);
      //Serial.println(mDirection);
   }
}

//return true ore false, depending on the direction the track should move
bool getDirection(int newPos){
    int directionV = pos - newPos;
    
    //negative Direction
    if (directionV >=0) {
        return 1; 
    }
    //positive Direction
    else{
        return 0;
    }
  }
