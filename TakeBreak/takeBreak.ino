/*
 Take Break Program : This program is used to take break from work after sometime continuously at office. 
 
 Author : Sashi Kumar Penta
 
 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino pin 7 
 Trig to Arduino pin 8
 
 Reference:
 Ultrasonic distance example is used from :http://arduinobasics.blogspot.com/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html 
 */


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

int inThreshold = 100; // 100 count means : 100 * t = 100 * 50 = 5000 ms = 5 seconds 
                       // (For demo we are saying take break after every 5 seconds, in reality 
                       // this number will be much higher)
                       
int outThreshold = 10; // This is much small.. just to avoid accidental readings.

void setup() {
 Serial.begin (9600);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
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
     if(inCount > inThreshold) // 50ms * 100 = 5000ms = 5 seconds (Break after 5 seconds)
     {
       outCount = 0;
       digitalWrite(LEDPin, HIGH); 
     }
   }
   else
   {
     outCount++;
     
     if(outCount > outThreshold) // 10 for noise???
     {
       inCount = 0;
       digitalWrite(LEDPin, LOW);
     }
   }
 }
 
 //Delay 50ms before next reading.
 delay(t);
}
