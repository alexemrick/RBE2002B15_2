
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
#include <Wire.h>
#include <L3G.h>
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

L3G gyro;

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


float slaveEncValue = 0, masterEncValue = 0, distanceTraveled = 0;
float encoderConversion = 8.6393 / 300;

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

const int flameIsClose = 970; //flame sensor value if it's in the cone
const int flameIsHere = 22;  //flame sensor value if it's in line up to 8" away

//variables for gyro

float G_Dt = 0.005;  // Integration time (DCM algorithm)  We will run the integration loop at 50Hz if possible

long timer = 0; //general purpose timer
long timer1 = 0;

float G_gain = .0109375; // gyros gain factor for 250deg/sec
//This gain factor can be effected upto +/- %2 based on mechanical stress to the component after mounting.
// if you rotate the gyro 180 degress and it only show 170 this could be the issue.

float gyro_x; //gyro x val
float gyro_y; //gyro x val
float gyro_z; //gyro x val
float gyro_xold; //gyro cummulative x value
float gyro_yold; //gyro cummulative y value
float gyro_zold; //gyro cummulative z value
float gerrx; // Gyro x error
float gerry; // Gyro y error
float gerrz; // Gyro z error

//initial setup
void setup() {
  Serial.begin(115200);
  Serial3.begin(115200);
  Wire.begin(); // i2c begin
  pinMode(fanPin, OUTPUT);
  pinMode(leftEncoderAPin, INPUT);
  pinMode(leftEncoderBPin, INPUT);
  pinMode(rightEncoderAPin, INPUT);
  pinMode(rightEncoderBPin, INPUT);
  leftDrive.attach(leftMotorPin, 1000, 2000);
  rightDrive.attach(rightMotorPin, 1000, 2000);
  fan.attach(fanPin);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

  masterEnc.write(0);
  slaveEnc.write(0);

  //setup for gyro stuff

  if (!gyro.init()) // gyro init
  {
    Serial.println("Failed to autodetect gyro type! not connected");
    while (1);
  }
  delay(500);
  timer = micros(); // init timer for first reading
  gyro.enableDefault(); // gyro init. default 250/deg/s
  delay(1000);// allow time for gyro to settle
  Serial.println("starting zero, stay still for 10 seconds");
  for (int i = 1; i <= 2000; i++) { // takes 2000 samples of the gyro
    gyro.read(); // read gyro I2C call
    gerrx += gyro.g.x; // add all the readings
    gerry += gyro.g.y;
    gerrz += gyro.g.z;
    delay(5);
  }

  gerrx = gerrx / 2000; // average readings to obtain an error offset
  gerry = gerry / 2000;
  gerrz = gerrz / 2000;

}

//main loop
void loop() {
  xDistanceTraveled = 10;
  yDistanceTraveled = 5;
  float angle = readGyro();
  distOrientation(angle, xDistanceTraveled);
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
      break;
    case 1:

      if (distanceFront <= distanceToFrontWall)
      {
        state = 4;
      }
      else
      {
        state = 6;
      }
      break;
    case 2: //turn right

      turnRobot(1, 90);
      state = 0;
      break;
    case 3: //turn left

      turnRobot(2, 90);
      state = 0;
      break;
    case 4: //is it the candle

      if (analogRead(flameSensorPin) < flameIsHere)
      {
        state = 5; //the obstacle is the candle
      }
      else
      {
        state = 6; //the obstacle is not the candle
      }
      break;
    case 5: //it is the candle, blow out the candle

      runFan();
      break;
    case 6: //it is not the candle, there is a wall in front of you OR there is a gap to the right

      if (distanceRight >= distanceToRightWall) //if there is no obstacle to the right
      {
        state = 2; //turn right
      }
      else //if there is an obstacle to the right
        //maybe also check left just to be sure/faster. this will just turn 90 twice instead of 180
      {
        state = 3; //turn left
      }
      break;
  }
}
