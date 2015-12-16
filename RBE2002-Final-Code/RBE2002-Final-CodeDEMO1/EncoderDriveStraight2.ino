/*
 * Drives robot straight; 2 wheels, encoder on each
 */

/*
 * "ISR" for drive straight
 */
void pidEncoders() {
  delay(10);
  errorE = (masterEnc.read() - previousMaster) - (slaveEnc.read() - previousSlave);
  DErrorE = errorE - oldErrorE;
  IErrorE += errorE;
  oldErrorE = errorE;

  delay(100);
  POUTE = errorE * kpE + DErrorE * kdE + IErrorE * kiE;

  // reset encoders so we have fresh values every loop
  previousMaster = masterEnc.read();
  previousSlave = slaveEnc.read();
}



/*
 * This function drives the robot straight
 */
void encoderDriveStraight() {
  // sets up timer and interrupt
  //  for (int i = 0; i < 1; i++) {
  //    masterEnc.write(0);
  //    slaveEnc.write(0);
  //
  //    // run motors
  //    leftDrive.write(masterPower);
  rightDrive.write(slavePower);
  //  }

  // calculates the PID
  pidEncoders();
  Serial.println(POUTE);
  // leftDrive.write(slavePower - POUTE);

  if (POUTE  > 23)
  {
    leftDrive.write(79);
  }
  else
  {
    leftDrive.write(masterPower - POUTE); //- with 2 ultras
  }
}
