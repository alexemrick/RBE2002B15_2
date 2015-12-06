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
  while(flameSensorValue < definiteFlame + 28 && flameSensorValue > definiteFlame - 22) fan.write(0);  // or HIGH if we do it that way
  fan.write(90);
}
