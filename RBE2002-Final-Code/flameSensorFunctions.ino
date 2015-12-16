/*
 * This function determines if the flame sensor is within 60 degrees of the robot and within 15"
 * 
 * This function is called while the robot is driving to see if the candle is anywhere close.
 * 
 * inputs: int flameValue - analog reading from flame sensor
 * outputs: boolean flameClose - if the flame is within the cone described above, return true
 * 
 */
boolean flameClose()
{
  float flameValue = analogRead(flameSensorPin); //read the flame sensor reading
  boolean present = false;
  if(flameValue < possibleFlame) present = true;
  else present = false;
  return present;
}

void foundFlame() {
  stopRobot();
  delay(100);
  rotateUntilHot();
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
  int pastSensorValue = flameSensorValue;

//  lcd.print("rotating");
  
  rotate(70);
  // If at certain flame sensor value away from candle, stop
  if(flameSensorValue <= 860 && flameSensorValue >= 0) {
    
    stopRobot();
    delay(1000);
    rightDrive.write(90);
    leftDrive.write(90);
   

  // check to see if it's time to blink the LED; that is, if the
  // difference between the current time and last time you blinked
  // the LED is bigger than the interval at which you want to
  // blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
    runFan();
  }
}
  }
//
//
//
//  if (timerF + 500 <= millis()) {
//          timerF = millis();
//}
