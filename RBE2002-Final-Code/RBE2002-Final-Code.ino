
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

#define fanPin 24

#define leftMotorPin 8
#define rightMotorPin 9

#define ledPin 27

//global variables
float xDistanceTraveled = 0;
float yDistanceTraveled = 0;

float distanceRight2;
float distanceRight1;
float distanceFront;
float distanceRight;
float distanceLeft;

float distX;
float distY;

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
int masterPower = 65;
int slavePower = 65;
boolean keepGoing = true;

int state = 8; //0

// set encoders and motors
// master on left; slave on right; for robot front faces away from you
Encoder masterEnc(2, 3);    // interrupt pins available:
Encoder slaveEnc(18, 19);   // used[2, 3, 18, 19]

float slaveEncValue = 0, masterEncValue = 0, previousMaster = 0, previousSlave = 0, distanceTraveled = 0;
float encoderConversion = 8.6393 / 300;

// prepare values for P
// error: difference between master and slave encoders
// + if slave needs to speed up, - for slow down, if at same speed, = 0
double error = 0;
double oldError = 0;
double DError, IError, POUT;

double errorE = 0;
double oldErrorE = 0;
double DErrorE, IErrorE, POUTE;

// decides how much the difference in encoder values effects
// the final power change to the motor
// final values: kp = 0.01; ki = 1.8; kd = 0.7;
const float kpE = 8.0;//.01
const float kiE = 0;//1.8
const float kdE = -1.4;//-.3;//0.7;

const float kp = 1.0;//0.5;
const float ki = 0.0;//0.0;
const float kd = -0.005;//0;

//NEVER  TOUCH THESE NEXT THREE NUMBERS
const float distanceToFront = 10.0;
const float rightObstacleDistance = 40.0;
const float distanceR = 7.5;

const int Stop = 90;

int ledState = LOW;             // ledState used to set the LED
const long timeInterval = 500;

const int possibleFlame = 900; //flame sensor value if it's in the cone
const int definiteFlame = 22;  //flame sensor value if it's in line up to 8" away

//variables for gyro

float G_Dt = 0.005;  // Integration time (DCM algorithm)  We will run the integration loop at 50Hz if possible

long timer = 0; //general purpose timer
long timer1 = 0;

float G_gain = .010936; // gyros gain factor for 250deg/sec
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
  pinMode(ledPin, OUTPUT);
  leftDrive.attach(leftMotorPin, 1000, 2000);
  rightDrive.attach(rightMotorPin, 1000, 2000);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

  // sets led on for looking for candle
  //digitalWrite(ledPin, HIGH);

  pinMode(13, OUTPUT);

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
void loop()
{
//  state= 8;
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
  float angle;
  readUltrasonic();

  switch (state)
  {
    //  follow
    case 0:
    lcd.print("WALL FOLLOW");
      lcd.setCursor(0, 0);
      digitalWrite(27 , HIGH);
      driveStraight();
      if ( flameClose(analogRead(flameSensorPin))) {

        state = 10;
      }
      readUltrasonic();
      delay(100);

      if ((distanceFront <= distanceToFront) || (distanceRight >= rightObstacleDistance)) //if there is an obstacle in front or a gap to the right
      {
        digitalWrite(27, HIGH); //turn on the LED
        stopRobot();
        delay(100);//stop the robot
        state = 1;
      }

      else
      {
        state = 0; //keep following the wall
      }
      

      break;
    case 1:  //if the robot is stopped
      lcd.print("STOP");
      lcd.setCursor(0, 0);
      readUltrasonic(); //update front and right distance values
      digitalWrite(27, HIGH);
      readUltrasonic();
      if (distanceRight >= rightObstacleDistance) //there is a gap to the right
      {
        //        driveForward(70,70);
        //        delay(100);
        state = 6;
      }
      else if (distanceFront <= distanceToFront) //there is an obstacle in front
      {
        state = 4;
      }
      else
      {

        state = 0; //if the robot got to case 1 on accident, keep driving straight (double checks ultrasonics
      }
      break;

    case 2: //turn right
lcd.print("TURN RIGHT");
      lcd.setCursor(0, 0);
      digitalWrite(27, HIGH);
      angle = readGyro();
      turnRobot(1, angle);
      stopRobot();
     // state = 7;

      
      break;
    case 3: //turn left
      lcd.print("TURN LEFT");
      lcd.setCursor(0, 0);
      if ( flameClose(analogRead(flameSensorPin))) {

        state = 10;
      }
      digitalWrite(27, HIGH);
      angle = readGyro();
      turnRobot(2, angle);
      stopRobot();
      state = 0;

      break;

    case 4: //is it the candle
      lcd.print("FRONT OBSTACLE");
      lcd.setCursor(0, 0);
      readUltrasonic();
      if (distanceFront <= distanceToFront)
      {
        if (analogRead(flameSensorPin) < definiteFlame)
        {
          digitalWrite(27, HIGH);   // turn the LED on (HIGH is the voltage level)
          delay(1000);              // wait for a second
          digitalWrite(27, LOW);    // turn the LED off by making the voltage LOW
          delay(1000);
          state = 5; //the obstacle is the candle
        }
        else
        {
          digitalWrite(27, HIGH);
          state = 6; //the obstacle is not the candle
        }
      }
      else
      {
        state = 1;
      }

      break;

    case 5: //it is the candle, blow out the candle
      lcd.print("CANDLE FOUND");
      lcd.setCursor(0, 0);
      digitalWrite(27, LOW);
      delay(1000);
      digitalWrite(27, HIGH);
      delay(1000);
      digitalWrite(27, LOW);
      displayLCD();
      runFan();
      state = 9;
      break;

    case 6: //it is not the candle, there is a wall in front of you OR there is a gap to the right
      readUltrasonic();
      if (distanceRight >= rightObstacleDistance) //if there is no obstacle to the right
      {
        lcd.print("RIGHT GAP");
        lcd.setCursor(0, 0);
        state = 2; //state 2 plus if there is not wall

      }
      else if (distanceFront > distanceToFront)
      {
        lcd.print("FRONT GAP");
        lcd.setCursor(0, 0);
        state = 0;
      }
      else if ((distanceFront <= distanceToFront) && (distanceRight < rightObstacleDistance)) //if there is an obstacle to the right
        //maybe also check left just to be sure/faster. this will just turn 90 twice instead of 180
      {
        lcd.print("RIGHT&FRONT OBSTACLES");
        lcd.setCursor(0, 0);
        state = 3; //turn left
      }
      break;

    // driving around a wall, aka driving when there's no fall to follow
    case 7:
      lcd.print("BABY DICK WALL");
      lcd.setCursor(0, 0);
      angle = readGyro();
      readUltrasonic();
      if (distanceLeft >= rightObstacleDistance)
      {
        digitalWrite(27, HIGH);
        //  encoderDriveStraight();
        driveForward(70, 67);
        delay(3000); //just timing drive straight for now
        stopRobot();
        turnRobot(1, angle); //turn right
        stopRobot();
      }
      state = 0; //start wall following again

      break;

    // start, drive towards the wall in order to follow from a set distance
    case 8:
      lcd.print("CASE 8");
      lcd.setCursor(0, 0);
      encoderDriveStraight();
      //driveForward(70, 70);
      readUltrasonic();
      if (distanceFront <= 7.5)
      {
        stopRobot();
        delay(100);
        turnRobot(2, readGyro());
        stopRobot();
        delay(500);

      }
      state = 0;

      break;
       case 9:
      lcd.print("DONE");
      lcd.setCursor(0, 0);
      stopRobot();
    case 10:
      lcd.print("CANDLE CLOSE");
      lcd.setCursor(0, 0);
      rotateUntilHot();
      state = 5;

   
  }
  delay(10);
}
