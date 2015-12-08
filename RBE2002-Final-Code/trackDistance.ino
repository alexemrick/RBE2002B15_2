/*
 * Tracks the distance w/ 2 encoders and averages the values
 */
float slaveEncValue = 0, masterEncValue = 0, distanceTraveled = 0;
float encoderConversion = 8.6393/300;

/*
 * This function displays the total distance traveled as measured by the encoders onto the LCD.
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


void trackDistance()
{
  // master on left; slave on right; for robot front faces away from you
  // 1 rev = 90 ticks; circum. of wheel = 8.639 in.
   masterEncValue = -((float)masterEnc.read()*encoderConversion);
  slaveEncValue = -((float)slaveEnc.read()*encoderConversion);
  distanceTraveled = (masterEncValue - slaveEncValue) / 2;
  lcd.print((int)distanceTraveled);
  lcd.setCursor(0,0);
  
  //displayLCD(); // this will be used when the gyro is implemented to tell direction. until then just print distanceTraveled
}


