//Written by Joe St. Germain 12/1/15
// Simply keeps track of the gyro reading from an minimu9 using the microseconds clock.
// This code has been tested on 3 seperate seperate sets of hardware in serveral locations.
// the code results in less than 1 degree per minute drift when stationary.
//Testing included 3 consecutive trials of 3 minutes on each set of hardware in 3 places.
// the maxiumum error observeded with a steady voltage was 1.3 degress per minute on a single axis over the 27 test.
// Average error was .275 degrees per minute averaged across all 3 axis.


float readGyro() {

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
return gyro_z;
}

