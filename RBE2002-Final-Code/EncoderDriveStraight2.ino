/*
 * Drives robot straight; 2 wheels, encoder on each
 */
#include <Servo.h>
#include <Encoder.h>
#include <TimerOne.h>

// initialize variables
int masterPower = 60;
int slavePower = 60;

// set encoders and motors
// master on left; slave on right; for robot front faces away from you
Encoder masterEnc(2,3);     // interrupt pins available:
Encoder slaveEnc(18,19);    // used[2, 3, 18, 19], free[20, 21]
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
double kp = 0.01;
double ki = 1.8;
double kd = 0.7;

void setup() {
  // motor pins
  masterMotor.attach(9, 1000, 2000);  // right
  slaveMotor.attach(8, 1000 ,2000);   // left

  // reset encoders
  masterEnc.write(0);
  slaveEnc.write(0);

  // run motors
  masterMotor.write(masterPower);
  slaveMotor.write(slavePower);

  // initialize timer & attach interrupt
  Timer1.initialize(100000);
  Timer1.attachInterrupt(pid);
}

void pid() {
  error = masterEnc.read() - slaveEnc.read();
  DError = error - oldError;
  IError += error;
  oldError = error;
  
  // reset encoders so we have fresh values every loop
  masterEnc.write(0);
  slaveEnc.write(0);
}

void loop() {
  // calculates the PID
  POUT = error * kp + DError * kd + IError * ki;
  
  slaveMotor.write(slavePower + POUT);
}
