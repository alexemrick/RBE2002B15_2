/*
 * Drives robot straight; 2 wheels, encoder on each
 */
// initialize variables

// set encoders and motors
// master on left; slave on right; for robot front faces away from you


// prepare values for P
// error: difference between master and slave encoders
// + if slave needs to speed up, - for slow down, if at same speed, = 0
// decides how much the difference in encoder values effects
// the final power change to the motor
// final values: kp = 0.01; ki = 1.8; kd = 0.7;

// run motors

// initialize timer & attach interrupt

void driveStraight() {

  if (Serial3.available()) {
    rightDrive.write(slavePower + POUT);
    leftDrive.write(slavePower);
    readUltrasonic();
    pidUltrasonic();
    POUT = error * kp + DError * kd + IError * ki;
    if (POUT + slavePower > 80)
    {
      rightDrive.write(79);
    }
    else
    {
      rightDrive.write(slavePower + POUT);
    }
    doTrig(trackDistance(), readGyro());
    distOrientation(readGyro());
    displayLCD();
  }
}

void pidUltrasonic() {
  error = distanceRight - distanceR;
  DError = error - oldError;
  IError += error;
  oldError = error;
}





