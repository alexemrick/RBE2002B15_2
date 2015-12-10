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
void rotateUntilHot()
{
  float POUT = 0;
  pidForFlame();
//  delay(200); //this delay may need to be altered or removed
//  rotate(10);

  Serial.println(POUT);
//  rotate(90 - POUT);
//  if(flameSensorValue <= 50 && flameSensorValue >= 35) {
//    Serial.println("Stopped");
//    stopRobot();
//  }
    
//  //if smaller = the candle is closer, keep turning
//  if(flameSensorValue > analogRead(flameSensorPin)) rotate(10);
//  //if larger = the candle is farther away
//  else if(flameSensorValue < analogRead(flameSensorPin)) rotate(170);
//  //if at certain flame sensor value away from candle, stop
//  else if((flameSensorValue < (analogRead(flameSensorPin)) - 10)) stopRobot();
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
