#include <Servo.h>
#define flameSensorPin A0
#define fanPin 4

Servo fan;

const int possibleFlame = 970; //flame sensor value if it's in the cone
const int definiteFlame = 22;  //flame sensor value if it's in line up to 8" away

void setup() {
  Serial.begin(9600);
  pinMode(fanPin, OUTPUT);
//  fan.attach(fanPin);
}

void loop() {
  float flameSensorValue = analogRead(flameSensorPin); //read the flame sensor reading
//  Serial.println(analogRead(flameSensorPin));
  if(flameSensorValue < definiteFlame + 28 && flameSensorValue > definiteFlame - 22) {
    fan.write(0);  // or HIGH if we do it that way
    Serial.println("loop");
  }
  Serial.println("STAHP");
  fan.write(90);
//  digitalWrite(fanPin, HIGH);
//  delay(1000);
}
