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
