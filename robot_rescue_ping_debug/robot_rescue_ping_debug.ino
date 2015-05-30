const int dir_a = 12;
const int dir_b = 13;
const int brake_a = 9;
const int brake_b = 8;
const int pwm_a = 3;
const int pwm_b = 11;

const int echoPinF = 2;
const int trigPinF = 4;
const int echoPinL = 5;
const int trigPinL = 6;
const int echoPinR = 7;
const int trigPinR = 10;


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

  pinMode(trigPinL, OUTPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(trigPinF, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(echoPinR, INPUT);
  pinMode(echoPinF, INPUT);
}

int leftSpeed = 80;
int rightSpeed = 86;
const int turnSpeed = 25;

//int usbValue;
//usbValue = analogRead(usbCheck);
//Serial.println(usbValue);

void loop()
{
  long cmF = getDistance('F');
  debugUltrasonic("F", cmF);
  long cmL = getDistance('L');
  debugUltrasonic("L", cmL);
  long cmR = getDistance('R');
  debugUltrasonic("R", cmR);

  if (cmF <= 7) {
    //motorsStop();
    turn('L');
  } else {
    motorsWrite();
    straightLine(cmL, cmR);
  }
}
