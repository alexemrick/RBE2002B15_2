/*
 * This function rotates the robot 90, -90, or 180 degrees from the angle at which it starts.
 *
 * This function is used after the robot stops when it finds and obstacle or gap. This uses the rotate() function
 * to set the motors to the right speeds.
 *
 * inputs: int turn - state for state machine, 1,2,3 for 90, -90, and 180 degrees respectively
 * outputs: none
 */
void turnRobot (int turn, float angle)
{
  int ang = angle*100;
  switch (turn)
  {
    case 1: //turn right
      {
        int newAngle = readGyro()*100; //MAKE THIS READ THE GYRO
        while (newAngle < (ang - 9000))
        {
          rotate(20);
        }
      }
    case 2: //turn left
      {
        int newAngle = readGyro()*100; //MAKE THIS READ THE GYRO
        while (newAngle < (ang + 9000))
        {
          rotate(160);
        }
      }
    case 3: // u turn
      {
        int newAngle = readGyro()*100; //MAKE THIS READ THE GYRO
        while (newAngle < (ang - 18000))
        {
          rotate(20);
        }
      }

  }
}

/*
 * This function rotates the robot in place.
 *
 * This is used to tell the robot to tank turn around its center (one wheel goes forward, the other backward).
 * It is used after you have determined which way the robot should turn using the ultrasonic sensors (in
 * findCandle()).
 *
 * inputs: motorSpeed = value written to left motor
 * outputs: none
 */
void rotate(int motorSpeed)
{
  leftDrive.write(motorSpeed);
  rightDrive.write(180 - motorSpeed);
}

/*
 * This function drives the robot straight without using any sensors.
 *
 * This is used in the driveStraightUltra() function. The motor speeds are updated in that function to correct the drift
 * and keep the robot straight.
 *
 * inputs: int speedR, speedL = values to pass to the right and left motors respectively
 * outputs: none
 */
void driveForward(int speedR, int speedL)
{
  leftDrive.write(speedL);
  rightDrive.write(speedR);
}

/*
 *  This function stops the robot from moving by turning off the motors.
 *
 *  This function is used in findCandle() when an object is detected in front of the robot or a large distance
 *  is detected on the right. (case 0)
 *
 *  input: none
 *  output: none
 */
void stopRobot()
{
  leftDrive.write(Stop);
  rightDrive.write(Stop);
}
