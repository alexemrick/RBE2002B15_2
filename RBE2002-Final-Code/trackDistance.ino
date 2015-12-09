/*
 * Tracks the distance w/ 2 encoders and averages the values
 */
/* This function displays the total distance traveled as measured by the encoders onto the LCD.
 *
 * xDistanceTraveled and yDistanceTraveled are global variables that are set by the function that tracks the
 * dista
 *
 * inputs: none
 * outputs: none
 */
void displayLCD()
{
  sprintf(str1, "%f", xDistanceTraveled);
  lcd.print("X = ");
  lcd.print(xDistanceTraveled);
  lcd.setCursor(8, 2);
  sprintf(str2, "%f", yDistanceTraveled);
  lcd.print("Y = ");
  lcd.print(yDistanceTraveled);
  lcd.home();
}


float trackDistance()
{
  // master on left; slave on right; for robot front faces away from you
  // 1 rev = 90 ticks; circum. of wheel = 8.639 in.
   masterEncValue = -((float)masterEnc.read()*encoderConversion);
  slaveEncValue = -((float)slaveEnc.read()*encoderConversion);
  return (masterEncValue + slaveEncValue) / 2;
}


