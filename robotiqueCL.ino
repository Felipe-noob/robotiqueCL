/*
 * Main file
 * 
 * apply a duty cycle of 255/ref to the motor1 (PWM) where ref is a value obtained from the serial port (in ascii format) 
 * send back the actual position of the motor shaft (in pulses).
 * 
 */

// arduino mega


#include <MeMegaPi.h>
#include "MeRGBLineFollower.h"
#include "Motors.h"
#include "Encoders.h"
#include "pid.h"

MeRGBLineFollower RGBLineFollower(PORT_8,1);
int16_t offset = 0;

#define DT 50 // sampling period in milliseconds

#define BASE_SPEED 50

void setup()
{

  InitMotors();

  InitEncoders();
  RGBLineFollower.begin();

  // Setting line following sensitivity, which is used for adjusting line following response speed. The larger the value is,more sensitive it turns.
  RGBLineFollower.setKp(1);

  
  // initialization of the serial communication.
  Serial.begin(9600);
  Serial.setTimeout(10);

}

void loop()
{
  // Main loop
  waitNextPeriod();

  // TODO: look up the docs
  RGBLineFollower.loop();
  offset = RGBLineFollower.getPositionOffset();
   
  int u = pid(offset, DT);
  // Serial.println(offset);
  setRightMotorAVoltage(- (BASE_SPEED - u));
  setLeftMotorAVoltage(BASE_SPEED + u );

  // get the new reference from the serial port is any.
  // if (Serial.available() > 1) // something to read ?
  //  ref = Serial.parseInt();  // parse the value (ascii -> int)
}

void waitNextPeriod()
{
  static long LastMillis = 0;
  long timeToWait = DT - (millis() - LastMillis);
  if (timeToWait > 0)
    delay(timeToWait);
  LastMillis = millis();
}
