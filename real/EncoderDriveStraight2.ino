/*
 * Drives robot straight; 2 wheels, encoder on each
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

void encoderDriveStraight() {
  rightDrive.write(slavePower);

  // calculates the PID
  pidEncoders();

  if (POUTE  > 23)leftDrive.write(79);
  else leftDrive.write(masterPower - POUTE);
}
