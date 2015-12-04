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
  if(flameValue < flameIsClose)
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
  float flameSensorValue = analogRead(flameSensorPin); //read the flame sensor reading
  delay(200); //this delay may need to be altered or removed
  rotate(10);
  if(flameSensorValue > analogRead(flameSensorPin)) //if the new value is smaller, the candle is closer
  {
    rotate(10);                               //...so keep turning that direction
  }
  else    //if the new value is larger, the candle is farther away
  {
    rotate(170);
    if((flameSensorValue - analogRead(flameSensorPin)) - 10)  //
    {
      stopRobot();
    }
  }
  //save the higher flame sensor value and make that the setpoint in a PID loop. use the loop to turn the robot 
  //back to that flame sensor value. just an idea.
}

