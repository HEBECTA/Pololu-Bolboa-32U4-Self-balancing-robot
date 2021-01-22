#ifndef BALANCE_H
#define BALANCE_H

#include <LSM6.h>
#include <Balboa32U4.h>

// Take 100 measurements initially to calibrate the gyro.
#define CALIBRATION_ITERATIONS 1000

// This constant relates the angle to its rate of change for a
// robot that is falling from a nearly-vertical position or
// rising up to that position.  The relationship is nearly
// linear.  For example, if you have the 80mm wheels it should be
// about 140, which means that the angle in millidegrees is ~140
// times its rate of change in degrees per second; when the robot
// has fallen by 90 degrees it will be moving at about
// 90,000/140 = 642 deg/s.  See the end of Balancer.ino for one
// way to calibrate this value.
#define ANGLE_RATE_RATIO 140

// The balancing code is all based on a 100 Hz update rate; if
// you change this, you will have to adjust many other things.
#define UPDATE_TIME_MS 10

extern Balboa32U4Encoders encoders;
extern LSM6 imu;

extern int32_t distanceLeft;
extern int32_t speedLeft;
extern int32_t driveLeft;
extern int32_t distanceRight;
extern int32_t speedRight;
extern int32_t driveRight;


void init_gyro();

int32_t integrateGyro();

void balance();

void integrateEncoders();

#endif
