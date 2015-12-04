
/*FIX: CALL THIS UP INSTEAD OF X AND THEN MAKE A condition in this function TO DETERMINE IF THIS VALUE RETURNED
 * IS IN THE X OR Y BASED ON THE GYRO (very similar to distOrientation).
 *
 * This function finds the displacement in the x or y direction that the robot traveled using trigonometry.
 *
 * inputs: float dist = distance from encoder reading, float angle = angle from gyro
 * outputs: x displacement of robot
 */
void doTrig(float dist, float angle) //fix this, need to add tolerances, can't really use mod
{
 // int ang = angle*100;
 int ang = angle;
  if ((ang % 180) < 5 || (ang % 180) > 175)
  {
    distX = (dist * (sin((PI / 2) - (ang * (PI / 90)))));
  }
  else
    distY = (dist * (sin((ang * (PI / 90)))));

}

/*
 * This function takes the distance traveled measured by the encoders and determines the orientation of the robot
 * and adds that distance to either the x or y global variable depending on the gyro.
 *
 * The input distX in this function is the output from doTrigX
 *
 * input: distance the robot has traveled in inches as a float
 * output: none
 */
void distOrientation(int gyro)
{
  //add tolerances, this is just an outline!
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
