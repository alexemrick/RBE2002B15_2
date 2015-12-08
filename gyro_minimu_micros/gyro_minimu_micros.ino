//Written by Joe St. Germain 12/1/15
// Simply keeps track of the gyro reading from an minimu9 using the microseconds clock.
// This code has been tested on 3 seperate seperate sets of hardware in serveral locations.
// the code results in less than 1 degree per minute drift when stationary.
//Testing included 3 consecutive trials of 3 minutes on each set of hardware in 3 places.
// the maxiumum error observeded with a steady voltage was 1.3 degress per minute on a single axis over the 27 test.
// Average error was .275 degrees per minute averaged across all 3 axis.
#include <Wire.h>
#include <L3G.h>
L3G gyro;


float G_Dt = 0.005;  // Integration time (DCM algorithm)  We will run the integration loop at 200Hz if possible
long timer = 0; // timer for the gyro
long timer1 = 0; //timer for printig
float G_gain = .00875; // gyros gain factor for 250deg/sec
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


void setup() {
  Serial.begin(115200);//fastest transmission
  Wire.begin(); // i2c begin
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

  Serial.println(gerrx); // print error vals
  Serial.println(gerry);
  Serial.println(gerrz);
}

void loop() {

  if ((micros() - timer) >= 5000) { // wait for 5000 microseconds
    gyro.read(); // read gyro

    gyro_x = (gyro.g.x - gerrx) * G_gain; // offset by error then multiply by gyro gain factor
    gyro_y = (gyro.g.y - gerry) * G_gain;
    gyro_z = (gyro.g.z - gerrz) * G_gain;

    gyro_x = gyro_x * G_Dt; // Multiply the angular rate by the time interval
    gyro_y = gyro_y * G_Dt;
    gyro_z = gyro_z * G_Dt;

    gyro_x += gyro_xold; // add the displacment(rotation) to the cumulative displacment
    gyro_y += gyro_yold;
    gyro_z += gyro_zold;

    gyro_xold = gyro_x ; // Set the old gyro angle to the current gyro angle
    gyro_yold = gyro_y ;
    gyro_zold = gyro_z ;
    // flag = 0;
    timer = micros(); //reset timer

  }


  if ((millis() - timer1) >= 1000) // print the gyro values once per second
  {
    timer1 = millis();

    Serial.print("G ");
    Serial.print("X: ");
    Serial.print(gyro_x);
    Serial.print(" Y: ");
    Serial.print(gyro_y);
    Serial.print(" Z: ");
    Serial.println(gyro_z);
  }

}

