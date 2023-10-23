/*
 * Main file
 * 
 * apply a duty cycle of 255/ref to the motor1 (PWM) where ref is a value obtained from the serial port (in ascii format) 
 * send back the actual position of the motor shaft (in pulses).
 * 
 */

// arduino mega


#include <MeMegaPi.h>
// #include <Arduino.h>
// #include <Timer3.h>
#include "MeRGBLineFollower.h"
#include "Motors.h"
#include "Encoders.h"
#include "pid.h"
MeRGBLineFollower RGBLineFollower(PORT_8,1);
MeUltrasonicSensor ultraSensor(PORT_7);
int16_t offset = 0;

#define LEDPIN 2
#define DT 50 // sampling period in milliseconds

#define BASE_SPEED 70
int flagUltraSensor = 0;
bool flagObstacle = 0;

void setup()
{

  InitMotors();

  InitEncoders();
  // Timer3.initialize();



  RGBLineFollower.begin();

  // Setting line following sensitivity, which is used for adjusting line following response speed. The larger the value is,more sensitive it turns.
  RGBLineFollower.setKp(1);

  

  // initialization of the serial communication.
  Serial.begin(9600);
  Serial.setTimeout(10);

  // Led obstacle
  pinMode(2, OUTPUT);


}

void loop()
{
  // digitalWrite(2, LOW);
  // Main loop
  waitNextPeriod();
  // TODO: look up the docs
  RGBLineFollower.loop();
  offset = RGBLineFollower.getPositionOffset();
  // int distance = ultraSensor.distanceCm();
  // Serial.println(ultraSensor.distanceCm()); 
  int u = pid(offset, DT);
  if(offset > 100){
  Serial.print(offset);
  Serial.print(",");
  Serial.print(u);
  Serial.print(",");
  Serial.println(flagObstacle);
  }



  setRightMotorAVoltage(- (BASE_SPEED - u));
  setLeftMotorAVoltage(BASE_SPEED + u );

  if (flagUltraSensor == 2){
    int distance = ultraSensor.distanceCm();
    // Serial.println(distance);
    if (distance <= 20){
      digitalWrite(2,HIGH);
      flagObstacle = true;
    }
    else{
      digitalWrite(2,LOW);
      flagObstacle = false;
    }
    flagUltraSensor = 0;
  } else {
    flagUltraSensor++;
  }
  // digitalWrite(2, HIGH);


  // Serial.print("Time: ");
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
