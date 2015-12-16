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
int sequenceCounter = 0;
int seq2 = 0;
bool exec = false;
int highest = 0;

void driveStraight() {

  double val = analogRead(A3);
  Serial.println(val);

  if (exec) {
    if (sequenceCounter == 0) {
      if (flameClose()) {
        foundFlame();
      }       else {
        if (timerDR + 200 <= millis()) {

          sequenceCounter++;
          timerDR = millis();
        } else {
          leftDrive.write(slavePower);
          rightDrive.write(masterPower + 7);
        }
      }
    } else if (sequenceCounter == 1) {
      if (flameClose()) {
        foundFlame();
      }
      else {
        if (val > highest) {
          highest = val;
        }
        leftDrive.write(90);
        rightDrive.write(masterPower + 20);
        if (val < highest - 15) {
          timerDR = millis();
          exec = false;
          sequenceCounter = 0;
        }
      }
    } else if (flameClose()) {
      foundFlame();
    }
    else {
      if (val < 300) {
        exec = true;
        sequenceCounter = 0;
        timerDR = millis();
      }
    }
  } else {
    if (sequenceCounter == 0) {
      if (flameClose()) {
        foundFlame();
      }
      else {
        if (timerDR + 500 <= millis()) {
          timerDR = millis();
          pidUltrasonic();
          sequenceCounter++;
          Serial.println("here");
        } else {
          leftDrive.write(slavePower);
          rightDrive.write(masterPower + 3);
        }
      }
    } else {
      if (timerDR + 100 <= millis()) {
        timerDR = millis();
        Serial.println("here2");
        sequenceCounter = 0;
      } else {
        //driveForward();
        leftDrive.write(slavePower + POUT);
        rightDrive.write(masterPower);
      }
    }
  }
  doTrig(trackDistance(), readGyro());
  distOrientation(readGyro());
}


void pidUltrasonic() {
  double sensorValue = analogRead(A3);
  currVAL = sensorValue;
  error = currVAL - lastVAL;
  lastVAL = currVAL;
  DError = error - oldError;
  IError += error;
  oldError = error;
  POUT = error * kp + DError * -kd + IError * ki;
}


