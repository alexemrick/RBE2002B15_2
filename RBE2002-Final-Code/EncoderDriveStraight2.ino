/*
 * Drives robot straight; 2 wheels, encoder on each
 */
#include <UltrasonicSensor.h>
#include <UltrasonicSensorArray.h>
#include <Servo.h>
#include <Encoder.h>
#include <TimerOne.h>

// initialize variables
int masterPower = 60;
int slavePower = 60;

// set encoders and motors
// master on left; slave on right; for robot front faces away from you

float distanceLeft;
float distanceRight;
float distanceFront;
const float distanceR = 4;

#define leftMotorPin 8
#define rightMotorPin 9

float lastTime;
float currentTime;
const float loopTime = 20;

Servo leftDrive;
Servo rightDrive;

// prepare values for P
// error: difference between master and slave encoders
// + if slave needs to speed up, - for slow down, if at same speed, = 0
double error = 0;
double oldError = 0;
double DError, IError, POUT;
// decides how much the difference in encoder values effects
// the final power change to the motor
// final values: kp = 0.01; ki = 1.8; kd = 0.7;
double kp = 1.5; //0.01;
double ki = 0.003; //0;
double kd = 0.02; //1;

float old;

void setup() {
  // motor pins
  leftDrive.attach(leftMotorPin, 1000, 2000);  // left
  rightDrive.attach(rightMotorPin, 1000 , 2000);  // right

  // reset ultrasonic?
  Serial.begin(115200);
  Serial3.begin(115200);
  
 old = Serial3.readStringUntil(',').toFloat();

  // run motors
  leftDrive.write(masterPower);
  rightDrive.write(slavePower);

  // initialize timer & attach interrupt

}
void readUltrasonic() {

  distanceFront = Serial3.readStringUntil(',').toFloat();
  distanceLeft = Serial3.readStringUntil(',').toFloat();
  distanceRight = Serial3.readStringUntil('\n').toFloat();
  
 
}

void pid() {
 


  error = distanceRight - distanceR;
  DError = error - oldError;
  IError += error;
  oldError = error;

 

}

void driveStraight() {
  if(Serial3.available()){
  pid();
  
  old = distanceLeft;
 readUltrasonic();
  
  POUT = error * kp + DError * kd + IError * ki;
  rightDrive.write(slavePower + POUT);
  
 
  //if(distanceLeft > old + 1 || distanceLeft < old - 1 );
         

   //else if(distanceLeft < old + 1 || distanceLeft > old - 1){
        //     slaveMotor.write(slavePower);
    //   }

  
 

  }
  }





