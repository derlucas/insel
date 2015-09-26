#include <Servo.h>

#define T1        A0
#define T2        A1
#define T1LED      6
#define T2LED      7
#define LEFT      A3
#define RIGHT     A2
#define UP        A4
#define DOWN      A5 
#define K2         2
#define K3         3
#define K4         4
#define K5         5
#define PWM1       9
#define PWM2      10      

#define angle_min    0
#define angle_max  180 
#define angle_mid   90

Servo servo1;
Servo servo2;

int servo1_angle, servo2_angle;
int motorActive = 0;

void setup() {
  pinMode(T1, INPUT_PULLUP);
  pinMode(T2, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(T1LED, OUTPUT);
  pinMode(T2LED, OUTPUT);
  pinMode(K2, OUTPUT);
  pinMode(K3, OUTPUT);
  pinMode(K4, OUTPUT);
  pinMode(K5, OUTPUT);

  Serial.begin(9600);  

  servo1.attach(PWM1, 500, 2500);
  servo2.attach(PWM2, 500, 2500);
  servo1.write(angle_mid);
  servo2.write(angle_mid);
  
  servo1_angle = angle_mid;
  servo2_angle = angle_mid;
  

  stopMotor();
  selectMotor(0);
}

void stopMotor() {
  digitalWrite(K3, HIGH);
  digitalWrite(K4, HIGH);  
}

void startMotorForward() {
  digitalWrite(K4, LOW);
  digitalWrite(K3, HIGH);
}

void startMotorReverse() {
  digitalWrite(K4, HIGH);
  digitalWrite(K3, LOW);
}

void selectMotor(byte motor) {
  if(motor == 0) {
    digitalWrite(K2, HIGH);
    digitalWrite(K5, LOW);
    digitalWrite(T1LED, HIGH);
    digitalWrite(T2LED, LOW);
  } else {
    digitalWrite(K2, LOW);
    digitalWrite(K5, HIGH);
    digitalWrite(T1LED, LOW);
    digitalWrite(T2LED, HIGH);
  }
  motorActive = motor;
}

void servoLeft(byte servo) {
  if(servo == 0) {
    servo1_angle--;
    if(servo1_angle <= angle_min) servo1_angle = angle_min;
    servo1.write(servo1_angle);
  } else {
    servo2_angle--;
    if(servo2_angle <= angle_min) servo2_angle = angle_min;
    servo2.write(servo2_angle);
  }
}

void servoRight(byte servo) {
  if(servo == 0) {
    servo1_angle++;
    if(servo1_angle >= angle_max) servo1_angle = angle_max;    
    servo1.write(servo1_angle);
  } else {
    servo2_angle++;
    if(servo2_angle >= angle_max) servo2_angle = angle_max;
    servo2.write(servo2_angle);
  }
}

int leftct = 0;
int rightct = 0;

// For pushButtons:
int pushButton = 0;


void loop() {
  delay(1);
  
  if (digitalRead(T1) == LOW)  {
    if (pushButton != 1)  {
      pushButton = 1;
      selectMotor(0);
    }
  } else if (pushButton==1) {
    pushButton = 0; 
  } else if (digitalRead(T2) == LOW)  {
    if (pushButton != 2)  {
      pushButton = 2;
      selectMotor(1);
    }
  } else if (pushButton==2) {
    pushButton = 0; 
  }
  
  if (digitalRead(UP) == LOW)  {
    if (pushButton !=3)  {
      pushButton=3;
      startMotorForward();
    }
  } else if (pushButton==3) {
    stopMotor();
    pushButton = 0; 
  }
  
  if (digitalRead(DOWN) == LOW)  {
    if (pushButton !=4)  {
      pushButton=4;
      startMotorReverse();
    }
  } else if (pushButton==4) {
    stopMotor();
    pushButton = 0; 
  }
  
  if (digitalRead(LEFT) == LOW)  {
    leftct++;
    
    if(leftct >= 20) {
      servoLeft(motorActive);
      leftct = 0;
    }
  } else {
    leftct = 0; 
  }
  
  if (digitalRead(RIGHT) == LOW)  {
    rightct++;
   
    if(rightct >= 20) {
      servoRight(motorActive);
      rightct = 0;
    }
  } else {
    rightct = 0; 
  }
  
}

