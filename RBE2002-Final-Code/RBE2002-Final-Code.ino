
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
const float distanceToRightWall = 6.0;

const int Stop = 90;

const int flameIsClose = 970;
const int flameIsHere = 22;
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

  Timer1.initialize(100000);
  Timer1.attachInterrupt(readUltrasonic);
}

//main loop
void loop() {
  findCandle();
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
        // driveStraightUltra;
        /*
         * This chunk of code describes when the candle is in the 60 degree 15 inch cone
         * float flameSensorValue = analogRead(flameSensorPin);
        if(flameClose(flameSensorValue))
        {
        rotateUntilHot();
        }
         */
        if (distanceFront <= distanceToFrontWall || distanceRight >= distanceToRightWall)
        {
          stopRobot();
          state = 1;
        }
      }
    case 1:
      {
        if (distanceFront <= distanceToFrontWall)
        {
          state = 4;
        }
        else
        {
          state = 7;
        }
      }
    case 2: //turn right
      {
        turnRobot(1, 90);
        state = 0;
      }
    case 3: //turn left
      {
        turnRobot(2, 90);
        state = 0;
      }
    case 4: //is it the candle
      {
        float flameSensorValue = analogRead(flameSensorPin);
        if (flameSensorValue < flameIsHere)
        {
          state = 5;
        }
        else
        {
          state = 6;
        }
      }
    case 5: //it is the candle, blow out the candle
      {
        runFan();
      }
    case 6: //it is not the candle, there is a wall in front of you
      {
        state = 7;
      }
    case 7: //is there a gap to the right
      {
        if (distanceRight >= distanceToRightWall)
        {
          state = 2; //turn right
        }
        else
        {
          state = 3;
        }
      }

  }
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


