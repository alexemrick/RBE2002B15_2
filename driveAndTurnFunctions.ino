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


void loop() {
  // put your main code here, to run repeatedly:

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
