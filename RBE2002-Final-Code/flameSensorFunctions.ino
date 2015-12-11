/*
 * This function determines if the flame sensor is within 60 degrees of the robot and within 15"
 * 
 * This function is called while the robot is driving to see if the candle is anywhere close.
 * 
 * inputs: int flameValue - analog reading from flame sensor
 * outputs: boolean flameClose - if the flame is within the cone described above, return true
 * 
 */
boolean flameClose(int flameValue)
{
  boolean present = false;
  if(flameValue < possibleFlame)
  {
    present = true;
  }
  else
  {
    present = false;
  }
  return present;
}

/*
 * This function rotates the robot until it finds the candle by stopping when the flame sensor value goes from high 
 * to low.
 * 
 * inputs: none
 * outputs: none
 */
 // MARISSA IS TESTING THIS :)
void rotateUntilHot()
{
  // method 1
//  float POUT = 0;
//  pidForFlame();
//  delay(200); //this delay may need to be altered or removed
//  rotate(10);

//  Serial.println(POUT);
//  rotate(90 - POUT);
//  if(flameSensorValue <= 50 && flameSensorValue >= 35) {
//    Serial.println("Stopped");
//    stopRobot();
//  }

  // method 2
  float flameSensorValue = analogRead(flameSensorPin); //read the flame sensor reading
  float pastSensorValue = 0;
  //if smaller = the candle is closer, keep turning
  if(flameSensorValue > pastSensorValue) rotate(60);
  //if larger = the candle is farther away
  else if(flameSensorValue < pastSensorValue) rotate(120);
  //if at certain flame sensor value away from candle, stop
  else if(flameSensorValue <= 50 && flameSensorValue >= 35) stopRobot();
  pastSensorValue = flameSensorValue;
}
  //save the higher flame sensor value and make that the setpoint in a PID loop. use the loop to turn the robot 
  //back to that flame sensor value. just an idea.

// helper PID function for rotateUntilHot()
float pidForFlame() {
  int kp = 1;
  int ki = 0;
  int kd = 0;
  int error, DError, IError, oldError;
  float flameSensorValue = analogRead(flameSensorPin); //read the flame sensor reading
  error = flameSensorValue - 50; // what we want the robot to stop at 
  DError = error - oldError;
  IError += error;
  oldError = error;
  return POUT = error * kp + DError * kd + IError * ki;
}

void blinkLED() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}

