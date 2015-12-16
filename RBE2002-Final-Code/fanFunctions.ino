/*
 * This function turns on the fan when the flame sensor is reading a low value. The fan stays on until the flame
 * appears extinguished indicated by a high flame sensor reading. Once the flame sensor reading is sufficiently high,
 * the fan turns off.
 *
 * This function is called after you determine that the obstacle in front of the robot is the candle based on the
 * flame sensor.
 *
 * input: none
 * output: none
 */
void runFan()
{
  float flameSensorValue = analogRead(flameSensorPin); //read the flame sensor reading
  if(flameSensorValue < definiteFlame + 848 && flameSensorValue > definiteFlame - 842) {
    digitalWrite(fanPin, HIGH);
    delay(5000);
  }
  else 
  {
    digitalWrite(fanPin, LOW);
    digitalWrite(ledPin, LOW);
  }
  
}
