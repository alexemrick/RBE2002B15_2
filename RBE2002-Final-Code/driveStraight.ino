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
  leftDrive.write(slavePower + POUT);
  pidUltrasonic();
  Serial.println(error);
//  if (POUT  > 34)
//  {
//    rightDrive.write(masterPower);
//  }
//  else
//  {
    rightDrive.write(masterPower - POUT); 
//  }
  //doTrig(trackDistance(), readGyro());
  //distOrientation(readGyro());
  //displayLCD();
  // }
  //delay(100);
}


void pidUltrasonic() {
  readUltrasonic();
  error = (distanceRight - distanceLeft)/2;  // distanceRight - 7.5
  //error = distanceRight - distanceR;
  DError = error - oldError;
  IError += error;
  oldError = error;
  POUT = error * kp + DError * kd + IError * ki;
}



//void driveStraight() {
////  pidUltrasonic();
////  Serial.println(POUT);
//  readUltrasonic();
//  rightDrive.write(70);
//  if(distanceRight > distanceR) leftDrive.write(60);
//  else if(distanceRight < distanceR) leftDrive.write(120);
//  else if(distanceRight <= distanceR + 2.0 && distanceRight >= distanceR - 2.0) {
////    if(distanceLeft > distanceR) leftDrive.write(120);
////    else if(distanceLeft < distanceR) leftDrive.write(60);
////    else {
//      leftDrive.write(70);
//    }
//  }
////  leftDrive.write(slavePower + POUT);
////}

//void pidUltrasonic() {
//  readUltraSonic();
//  error = distanceRight-distanceR;
//  DError = error - oldError;
//  IError += error;
//  oldError = error;
//  POUT = error * kp + DError * kd + IError * ki;
//}





