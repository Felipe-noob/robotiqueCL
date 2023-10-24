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

#define LEDPIN 2
#define DT 50 // sampling period in milliseconds
#define BASE_SPEED 50

MeRGBLineFollower RGBLineFollower(PORT_8,1);
MeUltrasonicSensor ultraSensor(PORT_7);
int16_t offset = 0;
int turnCicles = 0;
int flagUltraSensor = 0;
bool flagObstacle = 0;
int prevTurn = 0;
bool flagCurve = false;
int curveTimeout = 0;
int curveCooldown = 0;

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
  pinMode(LEDPIN, OUTPUT);
}

void loop()
{
  waitNextPeriod();

  // TODO: look up the docs
  RGBLineFollower.loop();
  // int distance = ultraSensor.distanceCm();

  const int speed = turnCicles 
  ? 0.7 * BASE_SPEED 
  : flagCurve
  ? 40
  : BASE_SPEED;

  if(turnCicles) {
    offset *= 0.95;
    turnCicles--;
    flagCurve = false;
  } else if ((abs(offset) >= 130) && flagObstacle){
    offset = 405;
    prevTurn = offset;
    flagObstacle = false;
    turnCicles = 10;
    flagCurve = false;
  } else {
    // no obstacle
    offset = RGBLineFollower.getPositionOffset();
    Serial.println(offset);
    
    // detects curves
    if (abs(offset) > 270 && curveTimeout == 0 && curveCooldown == 0){
      flagCurve = true;
      curveTimeout = 8;
      curveCooldown = 70;
    } else if (curveTimeout > 0) {
      // the robot is still at the curve
      curveTimeout--;
      curveCooldown--;
      flagCurve = true;
    } else if (curveTimeout == 0 && curveCooldown > 0){
      // there is a cooldown between curves to avoid a false positive
      flagCurve = false;
      curveCooldown--;
    } else {
      // regular operation, straight line
      flagCurve = false;
    }
  }

  int u = pid(offset, DT, flagCurve);

  setRightMotorAVoltage(- (speed - u));
  setLeftMotorAVoltage(speed + u );

  if (flagUltraSensor == 2){
    // checks for obstacle every 3 cycles
    int distance = ultraSensor.distanceCm();
    // Serial.println(distance);
    if (distance <= 27){
      flagObstacle = true;
    }
    flagUltraSensor = 0;

  } else {
    flagUltraSensor++;
  }

  // debug if there is an obstacle
  // Serial.println(offset);
  digitalWrite(LEDPIN, flagObstacle || turnCicles);
}

void waitNextPeriod()
{
  static long LastMillis = 0;
  long timeToWait = DT - (millis() - LastMillis);
  if (timeToWait > 0)
    delay(timeToWait);
  LastMillis = millis();
}
