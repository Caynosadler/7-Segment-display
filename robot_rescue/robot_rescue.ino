#include <NewPing.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <Servo.h>

#define dir_a 12
#define dir_b 13
#define brake_a 9
#define brake_b 8
#define PWM_A 3
#define PWM_B 11

#define ECHO_PIN_F 2
#define TRIG_PIN_F 4
#define ECHO_PIN_L 5
#define TRIG_PIN_L 6
#define ECHO_PIN_R 7
#define TRIG_PIN_R 10
#define MAX_DISTANCE 300

// Store our compass as a variable.
HMC5883L compass;
// Record any errors that may occur in the compass.
int error = 0;

Servo claw;

void setup()
{
  // initialize serial communication:
  Serial.begin(9600);

  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);
  //pinMode(brake_a, OUTPUT);
  //pinMode(brake_b, OUTPUT);
  pinMode(PWM_A, OUTPUT);
  pinMode(PWM_B, OUTPUT);

  pinMode(TRIG_PIN_L, OUTPUT);
  pinMode(TRIG_PIN_R, OUTPUT);
  pinMode(TRIG_PIN_F, OUTPUT);
  pinMode(ECHO_PIN_L, INPUT);
  pinMode(ECHO_PIN_R, INPUT);
  pinMode(ECHO_PIN_F, INPUT);
  
  claw.attach(9);
    
  Serial.println("Starting the I2C interface.");
  Wire.begin(); // Start the I2C interface.
  
  Serial.println("Constructing new HMC5883L");
  compass = HMC5883L(); // Construct a new HMC5883 compass.
  
  Serial.println("Setting scale to +/- 1.3 Ga");
  error = compass.SetScale(1.3); // Set the scale of the compass.
 
  Serial.println("Setting measurement mode to continous.");
  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));
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
  moveClaw();
  //claw.write(33);
  //claw.write(70);
  bussula(true);
  //delay(50);
}

//float getCompass(bool debug) {
float bussula(bool debug ) {
  // Retrive the raw values from the compass (not scaled).
  MagnetometerRaw raw = compass.ReadRawAxis();
  // Retrived the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = compass.ReadScaledAxis();

  // Values are accessed like so:
  int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)

  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(scaled.YAxis, scaled.XAxis);

  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: 2� 37' W, which is 2.617 Degrees, or (which we need) 0.0456752665 radians, I will use 0.0457
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.0457;
  heading += declinationAngle;

  // Correct for when signs are reversed.
  if (heading < 0)
    heading += 2 * PI;

  // Check for wrap due to addition of declination.
  if (heading > 2 * PI)
    heading -= 2 * PI;

  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180 / M_PI;

  // Output the data via the serial port.
  if (debug)
    outputCompass(raw, scaled, heading, headingDegrees);

  // Normally we would delay the application by 66ms to allow the loop
  // to run at 15Hz (default bandwidth for the HMC5883L).
  // However since we have a long serial out (104ms at 9600) we will let
  // it run at its natural speed.
  // delay(66);
  return headingDegrees;
}

// Output the data down the serial port.
void outputCompass(MagnetometerRaw raw, MagnetometerScaled scaled, float heading, float headingDegrees) {
  Serial.print("Raw:\t");
  Serial.print(raw.XAxis);
  Serial.print("  ");
  Serial.print(raw.YAxis);
  Serial.print("  ");
  Serial.print(raw.ZAxis);
  Serial.print("  \tScaled:\t");

  Serial.print(scaled.XAxis);
  Serial.print("  ");
  Serial.print(scaled.YAxis);
  Serial.print("  ");
  Serial.print(scaled.ZAxis);

  Serial.print("  \tHeading:\t");
  Serial.print(heading);
  Serial.print(" Radians  \t");
  Serial.print(headingDegrees);
  Serial.println(" Degrees  \t");
}

int pos = 2;
void moveClaw() {
  for(pos = 2; pos < 75; pos += 1)  // goes from 0 degrees to 180 degrees
  {                                  // in steps of 1 degree
    claw.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for(pos = 75; pos>=2; pos-=1)     // goes from 180 degrees to 0 degrees
  {
    claw.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
