//Written by Joe St. Germain 8/1/15
// Simply keeps track of the gyro reading from an minimu9
// I reccommend going one step further with a
//complimentary filter with the accelerometer for greater accuracy over longer periods of time.

float readGyro()
{
  if ((millis() - timer) >= 5) // reads imu every 5ms
  {

    gyro.read(); // read gyro
    timer = millis(); //reset timer
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
  }

  if ((millis() - timer1) >= 1000) // prints the gyro value once per second
  {
    timer1 = millis();
  }
  return gyro_z;
}

