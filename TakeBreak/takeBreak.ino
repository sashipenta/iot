/*
 Take Break Program : This program is used to take break from work after sometime continuously at office. 
 
 Author : Sashi Kumar Penta
 
 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino pin 7 
 Trig to Arduino pin 8
 
 Parameters
   inTime (in seconds) : indicates in what time intervals you want to take break
   distanceThreshold (in cms) : how close to the sensor is considered that you are inside
   brightness (0-255) : brightness of the led strip
 
 This program can be found at : https://github.com/sashipenta/iot/blob/master/TakeBreak/takeBreak.ino
 
 Reference:
 Ultrasonic distance example is used from :
               http://arduinobasics.blogspot.com/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html 
 */

#include <Adafruit_NeoPixel.h>

#define PIN 6     // LED Strip Pin
#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin
#define LEDPin 13 // Onboard LED

int maximumRange = 200;  // Maximum range needed
int minimumRange = 0;    // Minimum range needed
long duration, distance; // Duration used to calculate distance (in cm)

int distanceThreshold = 20; // If the user is 20 cms or less, we keep countin inCount

int t = 50;
int inCount = 0;     // In office count - each count corresponds to t ms
int outCount = 0;    // Out count - just so to avoid accidental/noisy readings. 

int inTime = 900;      // in seconds

// This value is overwritten to inTime * 20 in setup
int inThreshold = 100; // 100 count means : 100 * t = 100 * 50 = 5000 ms = 5 seconds 
                       // (For demo we are saying take break after every 5 seconds, in reality 
                       // this number will be much higher)
                       
int outThreshold = 10; // This is much small.. just to avoid accidental readings.

int brightness = 20;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
 Serial.begin (9600);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
 
 strip.begin();
 strip.show(); // Initialize all pixels to 'off'
 
 inThreshold = inTime * 20;
}

// Fill the dots one after the other with a color
void lightStrip(int num, int high) {

  // Number of leds to light  
  int k = 0;
 
  k = (float) num * strip.numPixels() / high;
  
  // at least 1 pixel (to show you are in range)
  k = max(k, 1);
  // No more than number of pixels
  k = min(k, strip.numPixels());
  
  for(uint16_t i=0; i<= k; i++) {
      strip.setPixelColor(i, strip.Color(brightness, brightness, brightness));
  }
  
  for(uint16_t i=k; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0,0,0));
  }
  
  strip.show();
}

void lightStripOff() {
  
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0,0,0));
  }
  
  strip.show();
}

void lightStripBlink() {
  
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(brightness,0,0));
  }
  
  strip.show();
  
  delay(500);
  
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0,0,0));
  }
  
  strip.show();
}

void loop() {
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance = duration/58.2;
 
 if (distance >= maximumRange || distance <= minimumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 Serial.println("-1");
 digitalWrite(LEDPin, HIGH); 
 }
 else {
   /* Send the distance to the computer using Serial protocol, and
   turn LED OFF to indicate successful reading. */
   Serial.println(distance);
   if(distance < distanceThreshold)
   {
     inCount++;
     
     // Light as much progress we made
     lightStrip(inCount, inThreshold);
     
     if(inCount >= inThreshold) // 50ms * 100 = 5000ms = 5 seconds (Break after 5 seconds)
     {
       outCount = 0;
       digitalWrite(LEDPin, HIGH); 
       
       // Special blink
       lightStripBlink();
     }
   }
   else
   {
     outCount++;
     
     if(outCount > outThreshold) // 10 for noise???
     {
       inCount = 0;
       
       // away for some time no lights should be on
       lightStripOff();
       
       digitalWrite(LEDPin, LOW);
     }
   }
 }
 
 //Delay 50ms before next reading.
 delay(t);
}
