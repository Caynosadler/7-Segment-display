#include "Arduino.h"

int turnDelay = 500;  // varialvel responsavel pelo tempo de curva em milesegundos

//void motorsWrite(int l = 100, int r = 100, char = 'F')
void motorsWrite() {
  digitalWrite(brake_a, LOW); // vira para direita
  digitalWrite(brake_b, LOW);
  digitalWrite(dir_a, LOW);
  digitalWrite(dir_b, LOW);
  analogWrite(pwm_a, rightSpeed);
  analogWrite(pwm_b, leftSpeed);

  //  digitalWrite(brake_a, LOW); // trás
  //  digitalWrite(brake_b, LOW);
  //  digitalWrite(dir_a, HIGH);
  //  digitalWrite(dir_b, HIGH);
  //  analogWrite(pwm_a, 100);
  //  analogWrite(pwm_b, 100);
  //  delay(2000);
}

void motorsStop() {
  digitalWrite(brake_a, HIGH); // vira para direita
  digitalWrite(brake_b, HIGH);
  leftSpeed = 0;
  rightSpeed = 0;
}

void turn(char d) {
  if (d == 'L') {
    digitalWrite(dir_a, LOW);
    digitalWrite(dir_b, HIGH);
    //digitalWrite(brake_a, LOW); // vira para direita
    //digitalWrite(brake_b, LOW);
    analogWrite(pwm_a, rightSpeed);
    analogWrite(pwm_b, leftSpeed);
    delay(turnDelay);
    analogWrite(pwm_a, 0);
    analogWrite(pwm_b, 0);
  }

  if (d == 'R') {
    digitalWrite(dir_a, HIGH);
    digitalWrite(dir_b, LOW);
    //digitalWrite(brake_a, LOW); // vira para direita
    //digitalWrite(brake_b, LOW);
    analogWrite(pwm_a, rightSpeed);
    analogWrite(pwm_b, leftSpeed);
    delay(turnDelay);
    analogWrite(pwm_a, 0);
    analogWrite(pwm_b, 0);
  }
}

void straightLine(long cmL, long cmR) {
  if (cmL <= 20) {
    analogWrite(pwm_a, rightSpeed - turnSpeed);
  }
  
  if (cmR <= 20) { 
    analogWrite(pwm_b, leftSpeed - turnSpeed);
    }
}

int velocityControl(int cm) {
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
