#include <ArduinoRobot.h>
#include <Wire.h>
#include <SPI.h>

const int servoPin = D1;
int letra;

void setup() {
  pinMode(servoPin, OUTPUT);
  digitalWrite(servoPin, LOW);
  Serial.begin(9600);

}
void loop() {
  letra = Serial.read();
  if (letra == 'a') {
    servo_gira(1000, 20);
  }

  if (letra == 'b') {
    servo_gira(2000, 20);
  }
}

void servo_gira(int lado, int angulo) {
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(lado);
    digitalWrite(servoPin, LOW);
    delayMicroseconds(20000 - lado);
    delay(30);
  }
}

