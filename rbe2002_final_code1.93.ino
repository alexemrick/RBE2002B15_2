
/*
 * This program was written to operate the final robot for team 2 in RBE 2002 B term 2015. The robot drives forward
 * until an ultrasonic sensor senses an object. It uses the flame sensor to determine if the object is the candle
 * or another obstacle. If it is the candle, it turns on a fan to extinguish the candle. If not, it turns right or left
 * depending on which direction does not have an obstacle close, measured by ultrasonic sensors.
 *
 * Originally written by Alex Emrick, Marissa Bennett, Nicolette Vere; November 2015
 *
 * Other I/O pins
 * - I/O pins 40,41,42,43,44,45 are used for connecting to data lines D4-D7 and control lines on the LCD
 * - LED pins for flame detection on pin 22
 * - Fan control on pin 4
 *
 * Motor pins
 * - VEX motor for left drive on pin 8
 * - VEX motor for right drive on pin 9
 *
 * Sensor Pins
 * - Ultrasonics input on pin 3
 * - Ultrasonics output on pin 2
 * - Left Encoder channel A on interrupt port 18
 * - Left Encoder channel B on interrupt port 19
 * - Right Encoder channel A on interrupt port 21
 * - Right Encoder channel B on interrupt port 20
 * - Flame sensor on port A0
 *
  */

//libraries
#include <Encoder.h>  //provided by Arduino
#include <Servo.h>    //provided by Arduino
#include <TimerOne.h> //provided by Arduino
#include <LiquidCrystal.h> //include the LCD library

//i/o, motor, and sensor pin constants
#define flameSensorPin A0
#define leftEncoderAPin 18
#define leftEncoderBPin 19
#define rightEncoderAPin 21
#define rightEncoderBPin 20

#define fanPin 4

#define leftMotorPin 8
#define rightMotorPin 9

#define ledPin 22

//global variables
float xDistanceTraveled = 0;
float yDistanceTraveled = 0;

float distanceLeft;
float distanceRight;
float distanceFront;

float distX;
float distY;

int state = 0;

LiquidCrystal lcd(40, 41, 42, 43, 44, 45);

Servo leftDrive;
Servo rightDrive;
Servo fan;

char str1[8]; //8?
char str2[8];

/*
 * Variables for driving straight
 */
// initialize variables
int masterPower = 60;
int slavePower = 60;
boolean keepGoing = true;

// set encoders and motors
// master on left; slave on right; for robot front faces away from you
Encoder masterEnc(2, 3);    // interrupt pins available:
Encoder slaveEnc(18, 19);   // used[2, 3, 18, 19], free[20, 21]

// prepare values for P
// error: difference between master and slave encoders
// + if slave needs to speed up, - for slow down, if at same speed, = 0
double error = 0;
double oldError = 0;
double DError, IError, POUT;

// decides how much the difference in encoder values effects
// the final power change to the motor
// final values: kp = 0.01; ki = 1.8; kd = 0.7;
const double kp = 0.01;
const double ki = 1.8;
const double kd = 0.7;

const float distanceToFrontWall = 7.0;
const float distanceToWallRight = 6.0;


//initial setup
void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
  pinMode(fanPin, OUTPUT);
  pinMode(leftEncoderAPin, INPUT);
  pinMode(leftEncoderBPin, INPUT);
  pinMode(rightEncoderAPin, INPUT);
  pinMode(rightEncoderBPin, INPUT);
  leftDrive.attach(leftMotorPin, 1000, 2000);
  rightDrive.attach(rightMotorPin, 1000, 2000);
  fan.attach(fanPin);

  lcd.begin(16, 2);
}

//main loop
void loop() {
  
}

/*
 * This function is the main state machine of the program represented by the Flow Chart in the drive entitled
 * "Find Candle". It incorporates all of the helper functions written in this file and will be called in the main
 * loop.
 *
 * inputs: none
 * outputs: none
 */
void findCandle()
{
  readUltrasonic();
  
  switch (state)
  {
    case 0:
      {
        driveStraightUltra;
        
        if (distanceFront <= 5.0 || distanceRight >= 14.0)
        {
          //lcd.print("STOP");
          stopRobot();
          state = 1;
        }
      }
    case 1:
      {
        if(distanceFront <= distanceToFrontWall)
        {
          state = 4;
        }
        
      }
    case 2:
      {
        turnRobot(1, 90);
      }
    case 3:
      {
        turnRobot(2, 90);
      }
    case 4: //is it the candle
    {
      float flameSensorValue = analogRead(flameSensorPin);
      if(flameSensorValue <
    }

  }
}

/*
 * This function reads the ultrasonic values sent over Serial3 from the Arduino Uno.
 *
 * This function is used in the findCandle() function and called in the first case.
 *
 * inputs: none
 * outputs: none
 *
 */
void readUltrasonic() {

  distanceFront = Serial3.readStringUntil(',').toFloat();
  distanceLeft = Serial3.readStringUntil(',').toFloat();
  distanceRight = Serial3.readStringUntil('\n').toFloat();
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
  leftDrive.write(90);
  rightDrive.write(90);
}

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
  int ang = angle * 100;
  if (ang % 18000 == 0)
  {
    distX = (dist * (sin((PI / 2) - (angle * (PI / 90)))));
  }
  else
    distY = (dist * (sin((angle * (PI / 90)))));

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

/*
 * This function turns on the fan when the flame sensor is reading a high value. The fan stays on until the flame
 * appears extinguished indicated by a low flame sensor reading. Once the flame sensor reading is sufficiently low,
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

}


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

/*
 * This function rotates the robot 90, -90, or 180 degrees from the angle at which it starts.
 *
 * This function is used after the robot stops when it finds and obstacle or gap. This uses the rotate() function
 * to set the motors to the right speeds.
 *
 * inputs: int turn - state for state machine, 1,2,3 for 90, -90, and 180 degrees respectively
 * outputs: none
 */
void turnRobot (int turn, int angle)
{
  switch (turn)
  {
    case 1: //turn right
      {
        float newAngle = 90; //MAKE THIS READ THE GYRO
        while (newAngle < (angle - 90))
        {
          rotate(20);
        }
      }
    case 2: //turn left
      {
        float newAngle = 90; //MAKE THIS READ THE GYRO
        while (newAngle < (angle + 90))
        {
          rotate(160);
        }
      }
    case 3: // u turn
      {
        float newAngle = 90; //MAKE THIS READ THE GYRO
        while (newAngle < (angle - 180))
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
 * This function drives the robot straight using the ultrasonic sensors. It measures the distance from the right and left walls twice.
 * If the right distance increases, the robot turns right slightly. If the left distance increases, the robot turns left slightly.
 *
 * This function will be case 0 of the main state machine.
 *
 * inputs: value passed to motors to drive straight
 * outputs: none
 */
void driveStraightUltra(int motorSpeed)
{
  const int kp = 0;
  const int ki = 0;
  const int kd = 0;

  int turning = 0;
  int speedR = motorSpeed;
  int speedL = motorSpeed;

  switch (turning)
  {
    case 0:
      {
        driveForward(speedR, speedL);

        float distanceL = Serial3.readStringUntil(',').toFloat();
        float distanceR = Serial3.readStringUntil('\n').toFloat();

        readUltrasonic();
        //add tolerances
        if ((distanceRight - distanceR) > 0)
        {
          turning = 1; //shift right
        }
        else if ((distanceLeft - distanceL) > 0)
        {
          turning = 2; //shift left
        }
      }
    case 1: //shift right
      {

      }
    case 2: //shift left
      {

      }
  }
}
/*
 * This function determines if the flame sensor is within 90 degrees of the robot and within 6"
 */

/*
 * This function returns the distance tracked by the encoders.
 *
 * Everytime the robot stops, this function is called. This value is the 'dist' input in the function
 * doTrig.
 * 
 * inputs:
 * outputs: float - distance in inches traveled
 *
 *
 */
float trackDistance()
{

}

