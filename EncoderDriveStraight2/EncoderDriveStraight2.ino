/*
 * Drives robot straight; 2 wheels, encoder on each
 */
#include <UltrasonicSensor.h>
#include <UltrasonicSensorArray.h>
#include <Servo.h>
#include <Encoder.h>
#include <TimerOne.h>

// initialize variables
int masterPower = 71;
int slavePower = 71;

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
double kp = 0.5; //0.01;
double ki = 0; //0;
double kd = 0.1; //1;

float old;

void setup() {
  // motor pins
  masterMotor.attach(8, 1000, 2000);  // left
  slaveMotor.attach(9, 1000 , 2000);  // right

  // reset ultrasonic?
  Serial.begin(9600);
  Serial3.begin(9600);
  
 old = Serial3.readStringUntil(',').toFloat();

  // run motors
  masterMotor.write(masterPower);
  slaveMotor.write(slavePower);

  // initialize timer & attach interrupt
Timer1.initialize(100000);
Timer1.attachInterrupt(pid);
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

void loop() {
  old = distanceLeft;
 readUltrasonic();
  
  POUT = error * kp + DError * kd + IError * ki;
  slaveMotor.write(slavePower - POUT);
  
 
  if(distanceLeft > old + 1 || distanceLeft < old - 1 );
         

   else if(distanceLeft < old + 1 || distanceLeft > old - 1){
             slaveMotor.write(slavePower);
       }

  
  Serial.print(distanceRight);
  Serial.print(", ");
  Serial.print(distanceRight - distanceR);
  Serial.print(", ");
  Serial.print(masterMotor.read());
  Serial.print(", ");
  Serial.println(slaveMotor.read());
}





