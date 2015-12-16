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
  switch (turn)
  {
    case 1: //turn right
      turning("right", angle);
      break;
    case 2: //turn left
      turning("left", angle);
      break;
    case 3: // u turn
      turning("uTurn", angle);
      break;
  }
}

/*
 * Helper function for deciding which way to turn
 * 
 * Inputs: String of "left", "right", or "uTurn"
 *         float angle of rotation
 * Outputs: None
 */
void turning(String key, float angle) {
  int ang = angle;
  int newAngle; //read the gyro
  newAngle = (int)readGyro();
  if(key.equals("uTurn")) {
    while (newAngle < (ang - 180)) {
      rotate(69);
      newAngle = readGyro(); //read the gyro
    }
  }
  if (newAngle > -10 && newAngle < 10){
    while (newAngle > -90) {
      if(key.equals("right")) rotate(69); //turn right
      else rotate(111); //turn left
      newAngle = readGyro(); //read the gyro
    }
  }
  else if (newAngle < -80 && newAngle > -100) {
    while (newAngle > -180){ 
      if(key.equals("right")) rotate(69); //turn right
      else rotate(111); //turn left
      newAngle = readGyro(); //read the gyro
    }
  }
  else if (newAngle > -190 && newAngle < -170) {
    while (newAngle > -270) {
      if(key.equals("right")) rotate(69); //turn right
      else rotate(111); //turn left
      newAngle = readGyro();
    }
  }
  else if (newAngle > -280 && newAngle < -260) {
    while (newAngle > -360) {
      if(key.equals("right")) rotate(69); //turn right
      else rotate(111); //turn left
      newAngle = readGyro();
    }
  }
  else if (newAngle > -370 && newAngle < -350) {
    while (newAngle > -450) {
      if(key.equals("right")) rotate(69); //turn right
      else rotate(111); //turn left
      newAngle = readGyro();
    }
  }
  else if (newAngle > 440 && newAngle < 460) {
    while (newAngle > 360) {
      if(key.equals("right")) rotate(69); //turn right
      else rotate(111); //turn left
      newAngle = readGyro();
    }
  }
  else if (newAngle > 350 && newAngle < 370) {
    while (newAngle > 270) {
      if(key.equals("right")) rotate(69); //turn right
      else rotate(111); //turn left
      newAngle = readGyro();
    }
  }
  else if (newAngle < 280 && newAngle > 260) {
    //while the gyro reading is less than 90 degrees to the right of the initial angle
    while (newAngle < 180) {
      if(key.equals("right")) rotate(69); //turn right
      else rotate(111); //turn left
      newAngle = readGyro(); //read the gyro
    }
  }
  else if (newAngle < 190 && newAngle > 170) {
    while (newAngle < 90) {
      if(key.equals("right")) rotate(69); //turn right
      else rotate(111); //turn left
      newAngle = readGyro(); //read the gyro
    }
  }
  else if (newAngle < 100 && newAngle > 80) {
    while (newAngle < 0) {
      if(key.equals("right")) rotate(69); //turn right
      else rotate(111); //turn left
      newAngle = readGyro(); //read the gyro
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

/*
 *  This function stops the robot from moving by turning off the motors.
 *
 *  This function is used to stop the robot 10 whatever's from the wall
 *
 *  input: none
 *  output: none
 */
void stopFromWall()
{
  readUltrasonic();
  if(distanceFront <= 10) stopRobot();
}
