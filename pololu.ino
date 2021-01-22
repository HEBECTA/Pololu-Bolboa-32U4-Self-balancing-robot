#include <Balboa32U4.h>
#include <LSM6.h>
#include <Wire.h>

#include "balance.h"

LSM6 imu;
#include <Balboa32U4Motors.h>
#include <Balboa32U4Encoders.h>

Balboa32U4Buzzer buzzer;
Balboa32U4Motors motors;
Balboa32U4Encoders encoders;

int motorSpeed = 0;

const char song[] PROGMEM =
  "!O6 T240"
  "l32ab-b>cl8r br b-bb-a a-r gr g-4 g4"
  "a-r gr g-gg-f er e-r d4 e-4"
  "gr msd8d8ml d-4d4"
  "l32efg-gl8r msd8d8ml d-4d4"
  "<bcd-d e-efg- ga-ab- a4 gr";

int mid = 110;

void playSong()
{
  if (!buzzer.isPlaying())
  {
    buzzer.playFromProgramSpace(song);
  }
}


void setup() {

  Serial.begin(115200);

  init_gyro();

  buzzer.playFromProgramSpace(song);

  delay(10);

  imu.read();
  int32_t y = integrateGyro();

  while( y < 80 || y > 110 ){


    delay(10);

    imu.read();
    y = integrateGyro();
  }

  Serial.println("starting !!!");

  //delay(1000);
}

void loop() {

 

  delay(10);

  imu.read();
  int32_t y = integrateGyro();

  //balance();

  //int16_t distanceDiff = distanceLeft - distanceRight;

   if ( y <= 40 )
    motorSpeed = map(y, 0, 90, 250, 10);

   else if ( y >= 40 && y <= mid )
    motorSpeed = map(y, 40, 90, 250, 10);

   else if ( y >= mid && y <= 120 )
    motorSpeed = map(y, 90, 130, -20, -250);

   else
    motorSpeed = map(y, 90, 180, -10, -250);

    Serial.println("speed: ");

    Serial.println(motorSpeed);
    
   motors.setSpeeds(motorSpeed, motorSpeed*0.8);

   //motors.setSpeeds(-40, 40);

}
