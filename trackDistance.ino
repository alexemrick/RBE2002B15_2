/*
 * Tracks the distance w/ 2 encoders
 */
#include <Encoder.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(40, 41, 42, 43, 44, 45);

float slaveEncValue = 0, masterEncValue = 0, distanceTraveled = 0;
float encoderConversion = 8.6393/300;

// set encoders and motors
// master on left; slave on right; for robot front faces away from you
Encoder masterEnc(2,3);     // interrupt pins available:
Encoder slaveEnc(18,19);    // used[2, 3, 18, 19], free[20, 21]

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0,0);

  // reset encoders
  masterEnc.write(0);
  slaveEnc.write(0);
}

void loop() {
  // 1 rev = 90 ticks; circum. of wheel = 8.639 in.
  masterEncValue = -((float)masterEnc.read()*encoderConversion);
  slaveEncValue = -((float)slaveEnc.read()*encoderConversion);
  distanceTraveled = (masterEncValue - slaveEncValue) / 2;
  lcd.setCursor(0,0);
  lcd.print((int)distanceTraveled);
}

