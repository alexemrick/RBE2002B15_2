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
 * - LED pins for flame detection on pin 27
 * - Fan control on pin 24
 *
 * Motor pins
 * - VEX motor for left drive on pin 5
 * - VEX motor for right drive on pin 4
 *
 * Sensor Pins
 * - Ultrasonics on Serial3
 * - Left Encoder channel A on interrupt port 2
 * - Left Encoder channel B on interrupt port 3
 * - Right Encoder channel A on interrupt port 18
 * - Right Encoder channel B on interrupt port 19
 * - Flame sensor on port A0
 *
 */

//libraries
#include <Encoder.h>  //provided by Arduino
#include <Servo.h>    //provided by Arduino
#include <LiquidCrystal.h> //include the LCD library
#include <Wire.h>
#include <L3G.h>

//i/o, motor, and sensor pin constants
#define flameSensorPin A0
#define fanPin 24
#define leftMotorPin 4
#define rightMotorPin 5
#define ledPin 27

//constructors
LiquidCrystal lcd(40, 41, 42, 43, 44, 45);

Servo leftDrive;
Servo rightDrive;

Encoder masterEnc(2, 3);
Encoder slaveEnc(18, 19);

L3G gyro;

//global variables
float xDistanceTraveled = 0;
float yDistanceTraveled = 0;

float distanceFront;
float distanceRight;
float distanceLeft;

float distX;
float distY;

char str1[8];
char str2[8];
unsigned long timerDR;
unsigned long timer = 0;
unsigned long timer1 = 0;
float lastVAL = 0;
float currVAL = 0;

int masterPower = 45;
int slavePower = 30;
int Stop = 90;
float slaveEncValue = 0, masterEncValue = 0, previousMaster = 0, previousSlave = 0, distanceTraveled = 0;
float encoderConversion = 8.6393 / 300;

double error = 0;
double oldError = 0;
double DError, IError, POUT = 0;

double errorE = 0;
double oldErrorE = 0;
double DErrorE, IErrorE, POUTE;

// PID for encoders
const float kpE = 8.0;
const float kiE = 0.0;
const float kdE = -1.4;

// PID for driving straight
const float kp = 11.0;
const float ki = 0.0;
const float kd = 0.0;

//NEVER TOUCH THESE NEXT THREE NUMBERS
const float distanceToFront = 10.0;
const float rightObstacleDistance = 40.0;
const float distanceR = 7.5;

const long timeInterval = 500;
int state;

const int possibleFlame = 900; //flame sensor value if it's in the cone
const int definiteFlame = 22;  //flame sensor value if it's in line up to 8" away

//variables for gyro
float G_Dt = 0.005;  // Integration time (DCM algorithm)  We will run the integration loop at 50Hz if possible

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
  timerDR = millis();
  Serial.begin(115200);
  Serial3.begin(115200);
  Serial.println("start");
  Wire.begin(); // i2c begin
  pinMode(fanPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(A3, INPUT);
  leftDrive.attach(leftMotorPin, 1000, 2000);
  rightDrive.attach(rightMotorPin, 1000, 2000);
  state = 0;

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

  // sets led on for looking for candle
  digitalWrite(ledPin, HIGH);

  //setup for gyro stuff
  if (!gyro.init()) // gyro init
  {
    Serial.println("Failed to autodetect gyro type! not connected");
    lcd.print("FAIL");
    while (1);
  }
  delay(500);
  timer = micros(); // init timer for first reading
  gyro.enableDefault(); // gyro init. default 250/deg/s
  delay(1000);// allow time for gyro to settle
  Serial.println("starting zero, stay still for 10 seconds");
  lcd.print("INIT");
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
  lcd.setCursor(0, 0);
  lcd.print("INIT COMPLETE");
  lcd.setCursor(0, 0);
}

//main loop
void loop()
{
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
    // wall follow
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("WALL FOLLOW");

      readUltrasonic();
      driveStraight();
      //if there is an obstacle in front or a gap to the right
      if (distanceFront <= distanceToFront) {
        stopRobot();
        delay(10);
        state = 1; //next case to make turning decision
      }
      else state = 0; //keep following the wall
      break;


    //if the robot is stopped and needs to turn
    case 1:  
      lcd.setCursor(0, 0);
      lcd.print("STOP");
      
      readUltrasonic(); //update front and right distance values

      // if there is a gap to the right
      if (distanceLeft >= rightObstacleDistance) {
        driveForward(50, 50);
        delay(1000);
        stopRobot();
        state = 2;
      }
      
      // there is an obstacle in front
      else if(distanceFront <= distanceToFront) state = 3;
      
      // if the robot got to case 1 on accident, keep driving straight 
      // (double checks ultrasonics)
      else state = 0; 
      break;


    //turn right to go around a wall
    case 2: 
      lcd.setCursor(0, 0);
      lcd.print("TURN RIGHT");

      angle = readGyro();
      turnRobot(1, angle);  // turn right
      stopRobot();
      delay(100);
      driveForward(60, 60);
      delay(1000);
      state = 7;
      break;
      

    //turn left; normal
    case 3: 
      lcd.setCursor(0, 0);
      lcd.print("TURN LEFT");

      angle = readGyro();
      turnRobot(2, angle);  // turns left
      stopRobot();
      delay(1000);
      state = 0;
      break;


    // check for candle
    case 4: 
      lcd.setCursor(0, 0);
      lcd.print("FRONT OBST");

      readUltrasonic();
      if (distanceFront <= distanceToFront) {
        //the obstacle is the candle
        if (analogRead(flameSensorPin) < definiteFlame) state = 5; 
        else state = 6; //the obstacle is not the candle
      }
      else state = 1; // state 0?
      break;
      

    //it is the candle, blow out the candle
    case 5: 
      lcd.setCursor(0, 0);
      lcd.print("CANDLE!!");

      stopRobot();
      displayLCD();
      runFan();
      state = 9;
      break;


    //it is not the candle, there is a wall in front of you AND/OR there is a gap to the right
    case 6: 
      readUltrasonic();
      //if there is no obstacle to the right
      if (distanceLeft >= rightObstacleDistance){
        lcd.setCursor(0, 0);
        lcd.print("GAP!!");
        state = 2; //state 2 plus if there is not wall
      }
      //if you're in a corner
      else if ((distanceFront <= distanceToFront) && (distanceLeft < rightObstacleDistance)) {
        lcd.setCursor(0, 0);
        lcd.print("WALL!!");
        state = 3; //turn left
      }
      //this case is just a double check, should not happen
      else if (distanceFront > distanceToFront) {
        state = 0;
      }
      break;
    //after turns right go back to wall following


    // driving around a wall, aka driving when there's no fall to follow
    case 7:
      lcd.setCursor(0, 0);
      lcd.print("BABY WALL");

      angle = readGyro();
      readUltrasonic();
      if (distanceLeft >= rightObstacleDistance) {
        stopRobot();
        turnRobot(1, angle); //turn right
        stopRobot();
      }
      state = 0; //start wall following again
      break;

    // start, drive towards the wall in order to follow from a set distance
    case 8:
      lcd.setCursor(0, 0);
      lcd.print("CASE 8");
      
      readUltrasonic();
      //while the robot is at least 7.5 inches away from the wall
      while (distanceFront > 7.5) {
        driveForward(40, 40);
        readUltrasonic();
      }
      stopRobot();
      delay(100);
      turnRobot(2, readGyro());
      stopRobot();
      delay(500);
      state = 0;
      break;

      
    //stop robot when we are done, stays here forever
    case 9:
      lcd.setCursor(0, 0);
      lcd.print("DONE");

      while(1) stopRobot();
      break;

      
    //when the candle is within the cone, turn straight towards it
    case 10:
      lcd.setCursor(0, 0);
      lcd.print("CANDLE CLOSE");

      rotateUntilHot();
      state = 5;
      break;
  }
}
