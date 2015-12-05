//Written by Joe St. Germain 8/1/15
// Simply keeps track of the gyro reading from an minimu9
// I reccommend going one step further with a
//complimentary filter with the accelerometer for greater accuracy over longer periods of time.
#include <Wire.h>
#include <L3G.h>
#include <LSM303.h>

L3G gyro;
LSM303 accel;

float G_Dt = 0.005;  // Integration time (DCM algorithm)  We will run the integration loop at 50Hz if possible

long timer = 0; //general purpose timer
long timer1 = 0;

float G_gain = .00875; // gyros gain factor for 250deg/sec
float gyro_x; //gyro x val
float gyro_y; //gyro x val
float gyro_z; //gyro x val
float gyro_xold; //gyro cummulative x value
float gyro_yold; //gyro cummulative y value
float gyro_zold; //gyro cummulative z value
float gerrx; // Gyro x error
float gerry; // Gyro y error
float gerrz; // Gyro 7 error

char report[80];

void setup() {
  Serial.begin(9600);
  Wire.begin(); // i2c begin

  gyro.init();
  gyro.enableDefault(); // gyro init. default 250/deg/s

  accel.init();
  accel.enableDefault();

  timer = millis(); // init timer for first reading

  delay(1000);// allow time for gyro to settle
  for (int i = 0; i < 100; i++) { // takes 100 samples of the gyro
    gyro.read();
    gerrx += gyro.g.x;
    gerry += gyro.g.y;
    gerrz += gyro.g.z;
    delay(25);
  }

  gerrx = gerrx / 100; // average reading to obtain an error/offset
  gerry = gerry / 100;
  gerrz = gerrz / 100;
}

void loop() {
  gyro.read(); // read gyro
  accel.readAcc();

  gyro_x = (float)(gyro.g.x - gerrx) * G_gain; // offset by error then multiply by gyro gain factor
  gyro_y = (float)(gyro.g.y - gerry) * G_gain;
  gyro_z = (float)(gyro.g.z - gerrz) * G_gain;

  gyro_x = gyro_x * G_Dt; // Multiply the angular rate by the time interval
  gyro_y = gyro_y * G_Dt;
  gyro_z = gyro_z * G_Dt;

  gyro_x += gyro_xold; // add the displacment(rotation) to the cumulative displacment
  gyro_y += gyro_yold;
  gyro_z += gyro_zold;

  gyro_xold = gyro_x ; // Set the old gyro angle to the current gyro angle
  gyro_yold = gyro_y ;
  gyro_zold = gyro_z ;

  snprintf(report, sizeof(report), "A: %6d %6d %6d    M: %6d %6d %6d",
           accel.a.x, accel.a.y, accel.a.z,
           gyro_x, gyro_y, gyro_z);
  Serial.println(report);

  delay(100);
}


