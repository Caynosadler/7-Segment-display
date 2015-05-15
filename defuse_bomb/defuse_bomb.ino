#include <ArduinoRobot.h>
#include <Wire.h>
#include <SPI.h>
#include <utility/RobotTextManager.h>
#include <avr/pgmspace.h>

/* HC-SR04 Sensor
   https://www.dealextreme.com/p/hc-sr04-ultrasonic-sensor-distance-measuring-module-133696
  
   This sketch reads a HC-SR04 ultrasonic rangefinder and returns the
   distance to the closest object in range. To do this, it sends a pulse
   to the sensor to initiate a reading, then listens for a pulse 
   to return.  The length of the returning pulse is proportional to 
   the distance of the object from the sensor.
     
   The circuit:
	* VCC connection of the sensor attached to +5V
	* GND connection of the sensor attached to ground
	* TRIG connection of the sensor attached to digital pin 2
	* ECHO connection of the sensor attached to digital pin 4


   Original code for Ping))) example was created by David A. Mellis
   Adapted for HC-SR04 by Tautvidas Sipavicius

   This example code is in the public domain.
 */
 
const int trigPin = D3;
const int echoPin = D1;

int stepCount = 1;
bool stepsEnd = false;
bool robotReady = false;
 
int robotSpeed;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  
  // initialize Robot and Display
  Robot.begin();
  Robot.beginTFT();
  Robot.beginSD();
  

  //Robot.waitContinue();
}

int motorSpeed;

void loop()
{
  
  //Serial.println(Robot.keyboardRead());
  
  Robot.setCursor(40, 50);
  Robot.print(getSpeedFromKnob(Robot.knobRead()));
  
  keyDown(Robot.keyboardRead());
  
  if (robotReady) {
    // establish variables for duration of the ping, 
    // and the distance result in inches and centimeters:
    long duration, inches, cm;
   
    // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    pinMode(trigPin, OUTPUT);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
      
    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
   
    // convert the time into a distance
    inches = microsecondsToInches(duration);
    cm = microsecondsToCentimeters(duration);
    
    Serial.print(inches);
    Serial.print("in, ");
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();
    
    switch(stepCount) {
      case 1:
        if (cm <= 5) {
          Robot.pointTo(45);
          stepCount++;
        }
        break;
      case 2:
        if (cm <= 5) {
          Robot.pointTo(-45);
          stepCount++;
        }
        break;
      case 3: {
        if (cm <= 65) {
          Robot.pointTo(-90);
          stepCount++;
          stepsEnd = true;
        }
      }
    }
    
    if (!stepsEnd) Robot.motorsWrite(robotSpeed, robotSpeed);
  }
  
  delay(50);
}

void keyDown(int keyCode) {
  switch (keyCode) {
    case BUTTON_LEFT:
      break;
    case BUTTON_RIGHT:
      break;
    case BUTTON_UP:
      break;
    case BUTTON_DOWN:
      break;
    case BUTTON_MIDDLE:
      robotSpeed = Robot.knobRead();
      robotReady = true;
      break;
    case BUTTON_NONE:
      break;
  }
}

int getSpeedFromKnob(int val) {
  static int val_old;
  int r = map(val, 0, 1023, 0, 255);

  //Only updates when the
  //value changes.
  if (val_old != r) {
    Robot.clearScreen();
    val_old = r;
  }
  
  return r;
}
 
long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}
 
long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 26.5 / 2;
}

int velocityControl(int cm)
{
  int mod;
  
  if (cm > 0 && cm <= 100) {
    mod = 0;
  } else if (cm > 100 && cm <= 200) {
    mod = 3;
  } else if (cm > 200 && cm <= 300) {
    mod = 2;
  } else if (cm > 300) {
    mod = 1;
  }
  
  return mod;
}
