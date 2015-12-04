/*
 * Drives robot straight; 2 wheels, encoder on each
 */
#include <UltrasonicSensor.h>
#include <UltrasonicSensorArray.h>
#include <Servo.h>
#include <Encoder.h>
#include <TimerOne.h>

// initialize variables
int masterPower = 70;
int slavePower = 70;

// set encoders and motors
// master on left; slave on right; for robot front faces away from you

float distanceLeft;
float distanceRight;
float distanceFront;
const float distanceR = 4;

float lastTime;
float currentTime;
const float loopTime = 20;

Servo masterMotor;
Servo slaveMotor;

// prepare values for P
// error: difference between master and slave encoders
// + if slave needs to speed up, - for slow down, if at same speed, = 0
double error = 0;
double oldError = 0;
double DError, IError, POUT;
// decides how much the difference in encoder values effects
// the final power change to the motor
// final values: kp = 0.01; ki = 1.8; kd = 0.7;
double kp = 0.05; //0.01;
double ki = 0; //0;
double kd = 0; //1;

void setup() {
  // motor pins
  masterMotor.attach(8, 1000, 2000);  // right
  slaveMotor.attach(9, 1000 , 2000);  // left

  // reset ultrasonic?
  Serial.begin(9600);
  Serial3.begin(9600);


  // run motors
  masterMotor.write(masterPower);
  slaveMotor.write(slavePower);

  // initialize timer & attach interrupt

}
void readUltrasonic() {

  distanceFront = Serial3.readStringUntil(',').toFloat();
  distanceLeft = Serial3.readStringUntil(',').toFloat();
  distanceRight = Serial3.readStringUntil('\n').toFloat();
}

void pid() {
  lastTime = currentTime;
  currentTime = millis();

  if (currentTime - lastTime > loopTime)
  {
    if (currentTime - lastTime < loopTime) {
      error - distanceRight - distanceR;
    }
    else {
      error = distanceR - distanceRight;
    }

  }
  error = distanceRight - distanceR;
  DError = error - oldError;
  IError += error;
  oldError = error;

  // distanceRight = 0;

}

void loop() {
  readUltrasonic();
  pid();

  POUT = error * kp + DError * kd + IError * ki;
  if (error < 0) {
    slaveMotor.write(slavePower - POUT);
  }
  else if (error > 0) {
    slaveMotor.write(slavePower + POUT);

  }
  Serial.print(distanceRight);
  Serial.print(", ");
  Serial.println(distanceRight - distanceR);
}





