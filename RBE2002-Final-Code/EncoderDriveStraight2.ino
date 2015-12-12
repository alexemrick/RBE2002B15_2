/*
 * Drives robot straight; 2 wheels, encoder on each
 */

/*
 * "ISR" for drive straight
 */
void pidEncoders() { 
  delay(10);
  errorE= (masterEnc.read() - previousMaster) - (slaveEnc.read() - previousSlave);
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
  for (int i = 0; i < 1; i++) {
    masterEnc.write(0);
    slaveEnc.write(0);

    // run motors
    leftDrive.write(masterPower);
    rightDrive.write(slavePower);

    // initialize timer & attach interrupt
//    Timer1.restart();
//    Timer1.initialize(100000);
//    Timer1.attachInterrupt(pidEncoders);
  }
  
  // calculates the PID
////<<<<<<< HEAD
//  POUTE = errorE * kpE + DErrorE * kdE + IErrorE * kiE;
//  leftDrive.write(slavePower + POUTE);
////  delay(100);
  pidEncoders();
  Serial.println(POUTE);
//=======
  leftDrive.write(slavePower + POUTE);
//}
//
///*
// *  This function stops the interrupt from enc drive straight
// */
//void stopEncDriveStr()
//{
//  Timer1.detachInterrupt();
//  Timer1.stop();
//>>>>>>> parent of 8898495... Working encoder drive straight-ish code
}
