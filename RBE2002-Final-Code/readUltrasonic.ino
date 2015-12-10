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
  delay(100);
    distanceFront = Serial3.readStringUntil(',').toFloat();
    distanceLeft = Serial3.readStringUntil(',').toFloat();
    distanceRight = Serial3.readStringUntil('\n').toFloat();
  
}
