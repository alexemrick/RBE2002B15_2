/*
 * Drives robot straight; 2 wheels, encoder on each
 */

// initialize timer & attach interrupt


void driveStraight() {

  double val = analogRead(A3);
  Serial.println(val);

  if (exec) {
    if (sequenceCounter == 0) {
      if (flameClose()) {
        foundFlame();
        doTrig(trackDistance(), readGyro());
        distOrientation(readGyro());
      }       else {
        if (timerDR + 200 <= millis()) {
          sequenceCounter++;
          doTrig(trackDistance(), readGyro());
          distOrientation(readGyro());
          timerDR = millis();
        } else {
          leftDrive.write(slavePower);
          rightDrive.write(masterPower + 7);
          doTrig(trackDistance(), readGyro());
          distOrientation(readGyro());
          //          displayLCD();
          //          lcd.print(trackDistance());
        }
      }
    } else if (sequenceCounter == 1) {
      if (flameClose()) {
        foundFlame();
        doTrig(trackDistance(), readGyro());
        distOrientation(readGyro());
      }
      else {
        if (val > highest) {
          highest = val;
          doTrig(trackDistance(), readGyro());
          distOrientation(readGyro());
        }
        leftDrive.write(90);
        rightDrive.write(masterPower + 20);
        doTrig(trackDistance(), readGyro());
        distOrientation(readGyro());
        displayLCD();
        if (val < highest - 15) {
          timerDR = millis();
          exec = false;
          sequenceCounter = 0;
          doTrig(trackDistance(), readGyro());
          distOrientation(readGyro());
        }
      }
    } else if (flameClose()) {
      foundFlame();
      doTrig(trackDistance(), readGyro());
      distOrientation(readGyro());
    }
    else {
      if (val < 300) {
        exec = true;
        sequenceCounter = 0;
        doTrig(trackDistance(), readGyro());
        distOrientation(readGyro());
        timerDR = millis();
      }
    }
  } else {
    if (sequenceCounter == 0) {
      if (flameClose()) {
        foundFlame();
        doTrig(trackDistance(), readGyro());
        distOrientation(readGyro());
      }
      else {
        if (timerDR + 500 <= millis()) {
          timerDR = millis();
          pidUltrasonic();
          sequenceCounter++;
          doTrig(trackDistance(), readGyro());
          distOrientation(readGyro());
          Serial.println("here");
        } else {
          leftDrive.write(slavePower);
          rightDrive.write(masterPower + 3);
          doTrig(trackDistance(), readGyro());
          distOrientation(readGyro());
          displayLCD();
          //lcd.print(trackDistance());
        }
      }
    } else {
      if (timerDR + 100 <= millis()) {
        timerDR = millis();
        Serial.println("here2");
        sequenceCounter = 0;
        doTrig(trackDistance(), readGyro());
        distOrientation(readGyro());
      } else {
        //driveForward();
        leftDrive.write(slavePower + POUT);
        rightDrive.write(masterPower);
        doTrig(trackDistance(), readGyro());
        distOrientation(readGyro());
        displayLCD();
        // lcd.print(trackDistance());
      }
    }
  }
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



