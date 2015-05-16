#include <ArduinoRobot.h>
#include <Wire.h>
#include <SPI.h>
#include <utility/RobotTextManager.h>
#include <avr/pgmspace.h>
 
const int trigPin = D3;
const int echoPin = D1;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  
  // initialize Robot and Display
  Robot.begin();
  Robot.beginTFT();
  Robot.beginSD();
  
  //Robot.waitContinue();
}


int stepCount = 1;
bool stepsEnd = false;
bool robotReady = false;
 
int robotSpeed;

void loop()
{  
  //Serial.println(Robot.keyboardRead());
  
  keyDown(Robot.keyboardRead());
  robotSpeed = getSpeedFromKnob(Robot.knobRead());
  
  if (robotReady) {
    // establish variables for duration of the ping, 
    // and the distance result in inches and centimeters:
    long duration, inches, cm;
    
    duration = getDuration();
   
    // convert the time into a distance
    inches = microsecondsToInches(duration);
    cm = microsecondsToCentimeters(duration);
    
    debugUltrasonic(inches, cm);
    //Serial.print(inches);
    //Serial.print("in, ");
    //Serial.print(cm);
    //Serial.print("cm");
    //Serial.println();
    
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
          //stepsEnd = true;
        }
      }
    }
    
    if (!stepsEnd) Robot.motorsWrite(robotSpeed, robotSpeed);
  }
  
  delay(50);
}

long getDuration() 
{
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
  
  return pulseIn(echoPin, HIGH);
}

void debugUltrasonic(int cm, int inches) 
{
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
}

void keyDown(int keyCode) 
{
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
      robotReady = true;
      break;
    case BUTTON_NONE:
      break;
  }
}


void printSpeedInfo(int speed, bool clear = false) 
{
    Robot.textSize(1);
    Robot.setCursor(40, 50);
    if (clear) { 
      Robot.stroke(255, 255, 255);
    } else {
      Robot.stroke(0, 0, 0);
    }
    //Robot.print(printf("Speed: %s", speed));
    Robot.print(speed);
}

int getSpeedFromKnob(int knob) 
{
  static int speed_old;
  int speed = map(knob, 0, 1023, 0, 255);
  printSpeedInfo(speed);

  //Only updates when the
  //value changes.
  if (speed_old != speed) {
    speed_old = speed;
    printSpeedInfo(speed, true);
  }
  
  return speed;
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
