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
  // initializes values
  //    for(int i = 0; i < 1; i++) {
  //      rightDrive.write(masterPower);
  //     // leftDrive.write(slavePower);
  //    }

  //if (Serial3.available()) {
  leftDrive.write(slavePower);
  //  rightDrive.write(masterPower + POUT);
  //    rightDrive.write(masterPower + POUT);
  // readUltrasonic();
  pidUltrasonic();

  if (POUT  > 23)
  {
    rightDrive.write(masterPower);
  }
  else
  {
    rightDrive.write(masterPower + POUT); //- with 2 ultras
  }
  doTrig(trackDistance(), readGyro());
  distOrientation(readGyro());
 // displayLCD();
  // }



  
}


void pidUltrasonic() {
  readUltrasonic();
 // error = distanceRight1 - distanceRight2;  // distanceRight - 7.5
  error = distanceRight - distanceR;
  DError = error - oldError;
  IError += error;
  oldError = error;
  POUT = error * kp + DError * kd + IError * ki;
}





