/*
 * Drives robot straight; 2 wheels, encoder on each
 */

// initialize variables

// set encoders and motors
// master on left; slave on right; for robot front faces away from you

// prepare values for P
// error: difference between master and slave encoders
// + if slave needs to speed up, - for slow down, if at same speed, = 0
double errorE = 0;
double oldErrorE = 0;
double DErrorE, IErrorE, POUTE;
// decides how much the difference in encoder values effects
// the final power change to the motor
// final values: kp = 0.01; ki = 1.8; kd = 0.7;
const float kpE = 0.01;
const float kiE = 1.8;
const float kdE = 0.7;

void setup() {
  // initialize timer & attach interrupt
  Timer1.initialize(100000);
  Timer1.attachInterrupt(pidEncoders);
}

void pidEncoders() {
  errorE = masterEnc.read() - slaveEnc.read();
  DErrorE = errorE - oldError;
  IErrorE += errorE;
  oldErrorE = errorE;
  
  // reset encoders so we have fresh values every loop
  masterEnc.write(0);
  slaveEnc.write(0);
}

void encoderDriveStraight() {
  // calculates the PID
  POUTE = errorE * kpE + DError * kdE + IError * kiE;
  
  rightDrive.write(slavePower + POUTE);
}
