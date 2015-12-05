
/* THIS FUNCTION IS USELESS IF WE GO PERFECTLY STRAIGHT
 * 
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
  if ((ang % 180) < 5 || (ang % 180) > 175) //if the angle is close to a multiple of 180 or 90
  {
    distX = (dist * (sin((PI / 2) - (ang * (PI / 90))))); //robot is in the x direction
  }
  else
    distY = (dist * (sin((ang * (PI / 90))))); //otherwise robot is in the y direction

}

/* IF DOTRIG IS SCRAPPED, THIS FUNCTION HAS TO CHANGE TO USE SOMETHING OTHER THAN DISTX AND DISTY.
 *  
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
  if (gyro > 0 && gyro < (PI / 2)) //if the robot is facing forward, the positive x direction
  {
    xDistanceTraveled += distX;
    yDistanceTraveled += distY;
  }
  else if (gyro > (PI / 2) && gyro < PI) //if the robot is facing left, the positive y direction
  {
    xDistanceTraveled -= distX;
    yDistanceTraveled += distY;
  }
  else if (gyro > PI && gyro < (3 * PI / 2)) //if the robot is facing backward, the negative x direction
  {
    xDistanceTraveled -= distX;
    yDistanceTraveled -= distY;
  }
  else if (gyro > (3 * PI / 2) && gyro < (2 * PI)) //if the robot is facing right, the negative y direction
  {
    xDistanceTraveled += distX;
    yDistanceTraveled -= distY;
  }
}
