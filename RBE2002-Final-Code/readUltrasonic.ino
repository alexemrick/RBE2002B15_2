/*
 * This function reads the ultrasonic values sent over Serial3 from the Arduino Uno.
 *
 * This function is used in the findCandle() function and called in the first case.
 *
 * inputs: none
 * outputs: none
 *
 */
void readUltrasonic() {
  while (!Serial3.available()) {}
  distanceFront = Serial3.readStringUntil(',').toFloat();
  //    distanceRight2 = Serial3.readStringUntil(',').toFloat();
  //    distanceRight1 = Serial3.readStringUntil('\n').toFloat();
  distanceLeft = Serial3.readStringUntil(',').toFloat();
  distanceRight = Serial3.readStringUntil('\n').toFloat();

}
