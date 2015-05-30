#include <NewPing.h>

#define dir_a 12
#define dir_b 13
#define brake_a 9
#define brake_b 8
#define pwm_a 3
#define pwm_b 11

#define ECHO_PIN_F 2
#define TRIG_PIN_F 4
#define ECHO_PIN_L 5
#define TRIG_PIN_L 6
#define ECHO_PIN_R 7
#define TRIG_PIN_R 10
#define MAX_DISTANCE 300

void setup()
{
  // initialize serial communication:
  Serial.begin(9600);

  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);
  pinMode(brake_a, OUTPUT);
  pinMode(brake_b, OUTPUT);
  pinMode(pwm_a, OUTPUT);
  pinMode(pwm_b, OUTPUT);

  pinMode(TRIG_PIN_L, OUTPUT);
  pinMode(TRIG_PIN_R, OUTPUT);
  pinMode(TRIG_PIN_F, OUTPUT);
  pinMode(ECHO_PIN_L, INPUT);
  pinMode(ECHO_PIN_R, INPUT);
  pinMode(ECHO_PIN_F, INPUT);
}

int leftSpeed = 80;
int rightSpeed = 86;
const int turnSpeed = 25;

//int usbValue;
//usbValue = analogRead(usbCheck);
//Serial.println(usbValue);

NewPing sonarFront(TRIG_PIN_F, ECHO_PIN_F, MAX_DISTANCE);
NewPing sonarLeft(TRIG_PIN_L, ECHO_PIN_L, MAX_DISTANCE);
NewPing sonarRight(TRIG_PIN_R, ECHO_PIN_R, MAX_DISTANCE);

void loop()
{
  unsigned int cmF = sonarFront.ping() / US_ROUNDTRIP_CM;
  debugUltrasonic("F", cmF);
  
  unsigned int cmL = sonarLeft.ping() / US_ROUNDTRIP_CM;
  debugUltrasonic("L", cmL);
  
  unsigned int cmR = sonarRight.ping() / US_ROUNDTRIP_CM;
  debugUltrasonic("R", cmR);

  //if (cmF <= 7) {
    //motorsStop();
    //turn('L');
  //} else {
    //motorsWrite();
    //straightLine(cmL, cmR);
  //}
  delay(50);
}
