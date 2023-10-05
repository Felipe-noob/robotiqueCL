/*
 * Main file
 * 
 * apply a duty cycle of 255/ref to the motor1 (PWM) where ref is a value obtained from the serial port (in ascii format) 
 * send back the actual position of the motor shaft (in pulses).
 * 
 */

// arduino mega

#include "Motors.h"
#include "Encoders.h"
#include "MeRGBLineFollower.h"

#define DT 50 //sampling period in milliseconds

uint8_t AD0 = 0;
uint8_t INT = 0;
uint8_t Address = 0;

MeRGBLineFollower RGBLineFollower(AD0, INT, Address);

void setup() {
  
 InitMotors();
 InitEncoders();
 
 // initialization of the serial communication.
 Serial.begin(9600);
 Serial.setTimeout(10);

 RGBLineFollower.begin();
 RGBLineFollower.setKp(0.3);
}


void loop() {
  // Main loop

  static int ref = 500; // reference signal
  
  int u = ref; // control signal
  
  waitNextPeriod();
  
  int position1 = getPosition1();  
  
  setMotorAVoltage(u);

  // send some values via serial ports (in ascii format)

  RGBLineFollower.loop(); 
  const int offset = RGBLineFollower.getPositionOffset();
  
  Serial.print(position1);
  Serial.print(",");
  Serial.print(u);
  Serial.print(",");
  Serial.print(offset);
  Serial.print(" ,");
  Serial.println(millis());

  // get the new reference from the serial port is any.
  if(Serial.available()>1) // something to read ?
    ref=Serial.parseInt(); // parse the value (ascii -> int)
}


void waitNextPeriod() 
{
  static long LastMillis=0; 
  long timeToWait = DT - ( millis() - LastMillis) ;
  if(timeToWait>0)
    delay(timeToWait);
  LastMillis=millis();
}
