
/* 
 * The trigonometry functions change depending on which way the robot is facing. To
 * accurately add the calculated displacements in each axis, this function is used.
 * This function finds the displacement in the x or y direction that the robot traveled using trigonometry.
 * This function decomposes the distance read from the encoders into its x and y components.
 *
 * inputs: float dist = distance from encoder reading, float angle = angle from gyro
 * outputs: x displacement of robot
 */
void doTrig(float dist, float angle) //fix this, need to add tolerances, can't really use mod
{
  // int ang = angle*100;
  int ang = angle;
  distX = (dist * (sin((PI / 2) - (ang * (PI / 180))))); //robot is in the x direction
  distY = (dist * (sin((ang * (PI / 180))))); //otherwise robot is in the y direction

}

/* 
 * This function takes the distance traveled measured by the encoders and determines the orientation of the robot
 * and adds that distance to either the x or y global variable depending on the gyro.
 *
 * The input distX in this function is the output from doTrig
 *
 * input: int gyro - angle from gyro in degrees
 * output: none
 */
void distOrientation(int gyro)
{
  //add tolerances?? i don't think you need them
  if (gyro > 0 && gyro < 90) //if the robot is facing forward, the positive x direction
  {
    xDistanceTraveled += distX;
    yDistanceTraveled += distY;
  }
  else if (gyro > 90 && gyro < 180) //if the robot is facing left, the positive y direction
  {
    xDistanceTraveled -= distX;
    yDistanceTraveled += distY;
  }
  else if (gyro > -90 && gyro < 0) //if the robot is facing backward, the negative x direction
  {
    xDistanceTraveled += distX;
    yDistanceTraveled -= distY;
  }
  else if ((gyro > -180 && gyro < -90) || (gyro < 270 && gyro > 180)) //if the robot is facing right, the negative y direction
  {
    xDistanceTraveled -= distX;
    yDistanceTraveled -= distY;
  }
}
