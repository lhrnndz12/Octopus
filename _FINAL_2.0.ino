

#include <dht.h>
#include <Wire.h>
#include "Adafruit_MCP9808.h"
#include <Servo.h>


//NEO PIXEL
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define NUMPIXELS      6
#define PIN            6
const int trigPin = 2;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//microphone
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
int sample;

Servo servo1;//create servo object to control a servo
Servo servo2;
Servo servo3;
Servo servo4;
dht DHT;
#define DHT11_PIN 7 // pin for humidity sensor 
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
int i = 0;


void setup() {
  //servo1.attach(5);//attachs the servo on pin 5 to servo object
  servo2.attach(4);
  servo3.attach(9);
  servo4.attach(10);
  servo1.write(0);//position at 0 degrees
  servo2.write(0);
  servo3.write(0);
  servo4.write(0);
  //pixels.setBrightness(50);
  //pixels.begin();
  Serial.begin(9600);
  while (!Serial); //waits for serial terminal to be open, necessary in newer arduino boards.
  Serial.println("MCP9808 demo");

  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1);
  }
  Serial.println("Found MCP9808!");
}

void loop() {
 
// //LED
//  for (int i = 0; i < NUMPIXELS; i++) {
//    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
//    pixels.show();
//  }


  ////Humidity
  //  int chk = DHT.read11(DHT11_PIN); // the follow is just so that I can see the readings come out properly
  // // Serial.print("Current humidity = ");
  // // Serial.print(DHT.humidity);
  // // Serial.print("%");
  // //  delay(800);
  // // Serial.println();
  //  if (DHT.humidity > 90) {
  //    for (i = 0; i < 177; i++) {
  //      servo1.write(i);
  //      delay(10);
  //    }
  //    for (i = 177; i > 0; i--) {
  //      servo1.write(i);
  //      delay(10);
  //    }
  //  } else {
  //    servo1.write(0);
  //  }



  //Temperature
  tempsensor.wake();   // wake up, ready to read!

  float c = tempsensor.readTempC();
  float f = tempsensor.readTempF();
  Serial.print("Temp: ");
  Serial.print(c, 4); Serial.print("*C\t and ");
  Serial.print(f, 4); Serial.println("*F.");
  Serial.println();
  if (f > 82) {
    for (i = 0; i < 177; i++) {
     // servo3.write(i);
      servo2.write(i);
      servo4.write(i);
      delay(10);
    }
    for (i = 177; i > 0; i--)
      //servo3.write(i);
    servo2.write(i);
    servo4.write(i);
    delay(10);
  } else {
    servo2.write(0);
    servo4.write(0);
  }



  //Microphone
  long startMillis = millis(); // Start of sample window
  int peakToPeak = 0;   // peak-to-peak level
  int signalMax = 0;
  int signalMin = 1024;

  //collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(0);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  double volts = (peakToPeak * 5.0) / 1024;  // convert to volts

  Serial.print("Mic :");
  Serial.println(volts);


  //MIC MOVEMENT
  if (volts > 1.35) {
    for (i = 0; i < 177; i++) {
      //servo4.write(i);
      servo3.write(i);
      delay(10);
    }
    for (i = 177; i > 0; i--) {
      servo3.write(i);
     // servo4.write(i);

      delay(10);
    }
  } else {
    servo3.write(0);
     //servo4.write(0);
  }
}
