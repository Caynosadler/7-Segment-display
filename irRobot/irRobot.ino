#include <ArduinoRobot.h>
#include <Wire.h>
#include <SPI.h>
#include <IRemote.h>

int RECV_PIN = D1;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop()
{
  if (irrecv.decode(&results))
    {
     Serial.println(results.value, HEX);
     irrecv.resume(); // Receive the next value
    }
}
