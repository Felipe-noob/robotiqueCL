/*
 * Main file
 * 
 * apply a duty cycle of 255/ref to the motor1 (PWM) where ref is a value obtained from the serial port (in ascii format) 
 * send back the actual position of the motor shaft (in pulses).
 * 
 */

// arduino mega


#include <MeMegaPi.h>
#include "Robot.h"


#define DT 30 // sampling period in milliseconds

MeRGBLineFollower RGBLineFollower(PORT_7,1);
CentraleUltrasonicSensor ultraSensor(PORT_8);
CentraleUltrasonicSensor carSensor(PORT_6);

Robot Corno(&RGBLineFollower, &ultraSensor, &carSensor,DT);

void setup()
{

  // Initializes Motors, Encoders, RGB Line follower and sets its KP according to the argument value
  Corno.init();

  // initialization of the serial communication.
  Serial.begin(9600);
  Serial.setTimeout(10);

  // Led obstacle
  pinMode(LEDPIN, OUTPUT);
}


void loop()
{
  waitNextPeriod();
  Corno.routine();
  // Corno.printInfo();
}


void waitNextPeriod()
{
  static long LastMillis = 0;
  long timeToWait = DT - (millis() - LastMillis);
  if (timeToWait > 0)
    delay(timeToWait);
  LastMillis = millis();
}

