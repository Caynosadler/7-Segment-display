#include <ArduinoRobot.h>
#include <Wire.h>
#include <SPI.h>
 
const int echoPinL = D4;
const int trigPinL = D5;
const int echoPinR = D2;
const int trigPinR = D0;
const int echoPinF = D1;
const int trigPinF = D3;

void setup() { 
  // initialize serial communication:
  Serial.begin(9600);
  
  // initialize Robot and Display
  Robot.begin();
  Robot.beginTFT();
  Robot.beginSD();
  
  pinMode(trigPinL, OUTPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(trigPinF, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(echoPinR, INPUT);
  pinMode(echoPinF, INPUT);
  
  //Robot.waitContinue();
}


int stepCount = 1;
bool stepsEnd = false;
bool robotReady = false;
 
int robotSpeed;

void loop()
{  
  robotSpeed = getSpeedFromKnob(Robot.knobRead());
  Robot.motorsWrite(robotSpeed, robotSpeed);
  
  long cmL = getDistance('L');
  debugUltrasonic("L", cmL);
  
  long cmR = getDistance('R');
  debugUltrasonic("R", cmR);
  
  long cmF = getDistance('F');
  debugUltrasonic("F", cmF);
  
  straightLine(cmL, cmR);
  
  delay(50);
}

void straightLine(long l, long r) {
  if (l <= 5) Robot.turn(0.5);
  if (r <= 5) Robot.turn(-0.5);
}

void makeTrig(int port) {
  digitalWrite(port, LOW);
  delayMicroseconds(2);
  digitalWrite(port, HIGH);
  delayMicroseconds(10);
  digitalWrite(port, LOW);
}

long getDistance(char p) 
{
  //microsecondsToInches(durationR);
  long echo;
  switch (p) {
    case 'L':
      makeTrig(trigPinL);
      echo = pulseIn(echoPinL, HIGH);
      return microsecondsToCentimeters(echo);
      break;
    case 'R':
      makeTrig(trigPinR);
      echo = pulseIn(echoPinR, HIGH);
      return microsecondsToCentimeters(echo);
      break;
    case 'F':
      makeTrig(trigPinF);
      echo = pulseIn(echoPinF, HIGH);
      return microsecondsToCentimeters(echo);
      break;
  }
}

void debugUltrasonic(String p, int cm)
{
  Serial.print(p + " ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
}

void debugUltrasonic(String p, int cm, int inches)
{
  Serial.print(p + " - ");
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
      Robot.setCursor(50, 60); 
      Robot.stroke(255, 255, 255);
      Robot.print("Ready");
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
