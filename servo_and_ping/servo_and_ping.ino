#include <ArduinoRobot.h>
#include <Wire.h>
#include <SPI.h>

const int servoPin = D1;
const int trigPin = D3; // trigPin D4 || 8
const int echoPin = D4; // echoPin D3 || 7
long cm;

void setup() {
  Robot.begin();
  pinMode(servoPin, OUTPUT);
  digitalWrite(servoPin, LOW);
  // initialize serial communication:
  Serial.begin(9600);
}

void loop() {
  //letra = Serial.read();
  //if (letra == 'a') {
    servo_gira(1000, 40);
  //}
  delay(2000);
  //if (letra == 'b') {
    servo_gira(2000, 80);
  //}
  
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration;

  // The PING))) is trigPingered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  
  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echoPin off of an object.
  pinMode(echoPin, INPUT);

  // convert the time into a distance
  cm = microsecondsToCentimeters(pulseIn(echoPin, HIGH));

  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(100);
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void servo_gira(int lado, int angulo) {
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(lado);
    digitalWrite(servoPin, LOW);
    delayMicroseconds(20000 - lado);
    delay(30);
}
