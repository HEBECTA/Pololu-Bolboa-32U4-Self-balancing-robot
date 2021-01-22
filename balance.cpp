#include <Wire.h>
#include "Balance.h"

int32_t gYZero;
int32_t angle; // millidegrees
int32_t angleRate; // degrees/s

int32_t distanceLeft;
int32_t speedLeft;
int32_t driveLeft;
int32_t distanceRight;
int32_t speedRight;
int32_t driveRight;

void integrateEncoders()
{
  static int16_t lastCountsLeft;
  int16_t countsLeft = encoders.getCountsLeft();
  speedLeft = (countsLeft - lastCountsLeft);
  distanceLeft += countsLeft - lastCountsLeft;
  lastCountsLeft = countsLeft;

  static int16_t lastCountsRight;
  int16_t countsRight = encoders.getCountsRight();
  speedRight = (countsRight - lastCountsRight);
  distanceRight += countsRight - lastCountsRight;
  lastCountsRight = countsRight;
}

void init_gyro(){
  
  Wire.begin();
  
  imu.init();

  imu.enableDefault();
  
  imu.writeReg(LSM6::CTRL2_G, 0b01011000); // 208 Hz, 1000 deg/s

  // Wait for IMU readings to stabilize.
  delay(1000);

  // Calibrate the gyro.
  int32_t total = 0;
  for (int i = 0; i < CALIBRATION_ITERATIONS; i++)
  {
    imu.read();
    total += imu.g.y;
    delay(1);
  }

  gYZero = total / CALIBRATION_ITERATIONS;
}

// Call this every 10ms (UPDATE_TIME_MS)
int32_t integrateGyro()
{
  // Convert from full-scale 1000 deg/s to deg/s.
  angleRate = (imu.g.y - gYZero) / 29;

  angle += angleRate * UPDATE_TIME_MS;

  //angle = angle * 999 / 1000;

  int32_t angle2 = angle / -1000;

  Serial.println(angle2);

  return angle2;
}

void balance(){


  //angle = angle * 999 / 1000;

  // This variable measures how close we are to our basic
  // balancing goal - being on a trajectory that would cause us
  // to rise up to the vertical position with zero speed left at
  // the top.  This is similar to the fallingAngleOffset used
  // for LED feedback and a calibration procedure discussed at
  // the end of Balancer.ino.
  //
  // It is in units of millidegrees, like the angle variable, and
  // you can think of it as an angular estimate of how far off we
  // are from being balanced.
  int32_t risingAngleOffset = angleRate * ANGLE_RATE_RATIO + angle;

  Serial.println(risingAngleOffset);
}
