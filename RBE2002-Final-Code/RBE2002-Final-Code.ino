
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

float distanceLeft;
float distanceRight;
float distanceFront;

float distX;
float distY;

LiquidCrystal lcd(40, 41, 42, 43, 44, 45);

Servo leftDrive;
Servo rightDrive;

L3G gyro;

char str1[8]; //8?
char str2[8];

String key = "";

/*
 * Variables for driving straight
 */
// initialize variables
int masterPower = 65;
int slavePower = 65;
boolean keepGoing = true;

int state = 8;

// set encoders and motors
// master on left; slave on right; for robot front faces away from you
Encoder masterEnc(18, 19);    // interrupt pins available:
Encoder slaveEnc(2, 3);   // used[2, 3, 18, 19]

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
const float kpE = 0.01;//1.75;
const float kiE = 1.8;//0.003;
const float kdE = 0.7;//-0.03;

const float kp = 1;
const float ki = 0.0;
const float kd = -0.005;

const float distanceToFront = 10.0;
const float rightObstacleDistance = 40.0;
const float distanceR = 10.0;

const int Stop = 90;

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
  pinMode(27, OUTPUT);
  leftDrive.attach(leftMotorPin, 1000, 2000);
  rightDrive.attach(rightMotorPin, 1000, 2000);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

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
  state = 0;
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
    case 0:
      angle = readGyro();
      turnRobot(1, angle);
      stopRobot();
      delay(4000);
    //      angle = readGyro();
    //      turnRobot(1, angle);
    //      stopRobot();
    //      delay(4000);
    //      angle = readGyro();
    //      turnRobot(1, angle);
    //      stopRobot();
    //      delay(4000);

    case 10:
      digitalWrite(27 , HIGH);
      driveStraight();
      /*
      * This chunk of code describes when the candle is in the 60 degree 15 inch cone
      * float flameSensorValue = analogRead(flameSensorPin);
      if(flameClose(flameSensorValue))
      {
      rotateUntilHot();
      }
      */
      readUltrasonic();
      delay(100);
      if ((distanceFront <= distanceToFront) || (distanceRight >= rightObstacleDistance))
      {
        digitalWrite(27, HIGH);
        stopRobot();
        state = 1;
      }
      else
      {
        state = 0;
      }
      break;

    case 1:
      readUltrasonic();
      digitalWrite(27, HIGH);
      if (distanceRight >= rightObstacleDistance) //there is a gap to the right
      {
        state = 6;
      }
      else if (distanceFront <= distanceToFront) //there is an obstacle in front
      {
        state = 4;
      }

      else
      {
        state = 0;
      }
      break;

    case 2: //turn right
      digitalWrite(27, HIGH);
      angle = readGyro();
      turnRobot(1, angle);
      stopRobot();
      state = 7;
      break;

    case 3: //turn left
      digitalWrite(27, HIGH);
      angle = readGyro();
      turnRobot(2, angle);
      stopRobot();
      state = 0;
      break;

    case 4: //is it the candle

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
      break;

    case 5: //it is the candle, blow out the candle
      displayLCD();
      runFan();
      digitalWrite(27, LOW);    // turn the LED off by making the voltage LOW
      state = 9;
      break;

    case 6: //it is not the candle, there is a wall in front of you OR there is a gap to the right

      if (distanceRight >= rightObstacleDistance) //if there is no obstacle to the right
      {
        digitalWrite(27, HIGH);
        state = 2; //state 2 plus if there is not wall
      }
      else if (distanceFront > distanceToFront)
      {
        state = 0;
      }
      else if ((distanceFront <= distanceToFront) && (distanceRight < rightObstacleDistance)) //if there is an obstacle to the right
        //maybe also check left just to be sure/faster. this will just turn 90 twice instead of 180
      {
        digitalWrite(27, HIGH);
        state = 3; //turn left
      }
      break;

    case 7:
      //      angle = readGyro();
      //      turnRobot(1, angle);
      //runs both motors for a bit so it drives straight
      readUltrasonic();
      if (distanceRight >= rightObstacleDistance)
      {
        digitalWrite(27, HIGH);
        encoderDriveStraight();
        delay(2000);
        stopRobot();
        turnRobot(1, angle);
        stopRobot();
      }
      state = 0;
      break;
    case 8:
      digitalWrite(27, HIGH);
      //  encoderDriveStraight();
      driveForward(75, 72);
      readUltrasonic();
      if (distanceFront <= distanceToFront)
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
      stopRobot();

  }
  delay(10);
}
