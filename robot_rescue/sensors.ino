#include "Arduino.h"

void makeTrig(int port) 
{
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

void debugUltrasonic(String p, int cm)
{
  Serial.print(p + " ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
}
