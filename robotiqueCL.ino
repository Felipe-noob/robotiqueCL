/*
 * Main file
 *
 * apply a duty cycle of 255/ref to the motor1 (PWM) where ref is a value obtained from the serial port (in ascii format)
 * send back the actual position of the motor shaft (in pulses).
 *
 */

// arduino mega


#include <MeMegaPi.h>
#include <MeRGBLineFollower.h>
#include "Motors.h"
#include "Encoders.h"

MeRGBLineFollower RGBLineFollower(PORT_7,1);
int16_t offset = 0;

#define DT 50 // sampling period in milliseconds



void setup()
{

  InitMotors();

  InitEncoders();
  RGBLineFollower.begin();

  // Setting line following sensitivity, which is used for adjusting line following response speed. The larger the value is,more sensitive it turns.
  RGBLineFollower.setKp(0.3);

  
  // initialization of the serial communication.
  Serial.begin(9600);
  Serial.setTimeout(10);

}

void loop()
{
  // Main loop

  RGBLineFollower.loop();
  offset = RGBLineFollower.getPositionOffset();
  Serial.println(offset);
  
  static int ref = 0; // reference signal

  int u = ref; // control signal

  waitNextPeriod();

  int position1 = getPosition1();
  u = 10;
  setRightMotorAVoltage(-20);
  setLeftMotorAVoltage(u);

  // send some values via serial ports (in ascii format)
  Serial.print(position1);
  Serial.print(",");
  Serial.print(u);
  Serial.print(",");
  Serial.print(millis());

  // get the new reference from the serial port is any.
  if (Serial.available() > 1) // something to read ?
    ref = Serial.parseInt();  // parse the value (ascii -> int)
}

void waitNextPeriod()
{
  static long LastMillis = 0;
  long timeToWait = DT - (millis() - LastMillis);
  if (timeToWait > 0)
    delay(timeToWait);
  LastMillis = millis();
}
