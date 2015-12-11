/*
 * Drives robot straight; 2 wheels, encoder on each
 */

/*
 * "ISR" for drive straight
 */
void pidEncoders() { 
  errorE= (masterEnc.read() - previousMaster) - (slaveEnc.read() - previousSlave);
  DErrorE = errorE - oldErrorE;
  IErrorE += errorE;
  oldErrorE = errorE;
  
  // reset encoders so we have fresh values every loop
  previousMaster = masterEnc.read();
  previousSlave = slaveEnc.read();
}

/*
 * This function drives the robot straight
 */
void encoderDriveStraight() {
  // sets up timer and interrupt
  for (int i = 0; i < 1; i++) {
    // reset encoders
    masterEnc.write(0);
    slaveEnc.write(0);

    // run motors
    leftDrive.write(slavePower);
    rightDrive.write(masterPower);
  }
  
  // calculates the PID
  POUTE = errorE * kpE - DErrorE * kdE + IErrorE * kiE;
  leftDrive.write(slavePower + POUTE);
//  delay(100);
  pidEncoders();
}
