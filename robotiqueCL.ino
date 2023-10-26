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
bool flagOtherPath = false;
int prevTurn = 0;
bool flagCurve = false;
int curveTimeout = 0;
int curveCooldown = 0;
int exitTimeout = 0;

int leftTurns = 0;
int rightTurns = 0; 

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

  InitEncoders();
}


void loop()
{
  waitNextPeriod();
  // TODO: look up the docs
  RGBLineFollower.loop();
  // int distance = ultraSensor.distanceCm();
  const int speed = flagOtherPath 
  ? 0.75 * BASE_SPEED 
  : flagCurve
  ? 40
  : BASE_SPEED;

  // left,right
  
  Serial.print(flagObstacle);
  Serial.print(",");
  Serial.print(flagOtherPath);
  Serial.print(",");
  Serial.print(leftTurns);
  Serial.print(",");
  Serial.print(rightTurns);
  Serial.print(",");
  Serial.print(getPosition1());
  Serial.print(",");
  Serial.println(getPosition2());

  if (flagObstacle){
    // the is an obstacle
    setRightMotorAVoltage(0);
    setLeftMotorAVoltage(110);
    flagOtherPath = true;    
    rightTurns = getPosition2();
    leftTurns = getPosition1();
  } else if (flagOtherPath) {
    // the robot is deviating from the obstacle

    // TODO:

    const int curveIndex = (getPosition1() - leftTurns) - (getPosition2() - rightTurns);
    if(curveIndex < -150) {
      // testado experimentalmente, refinar o código mais tarde
      flagOtherPath = false;
    }
    offset = RGBLineFollower.getPositionOffset();
    const int sensor_state = RGBLineFollower.getPositionState();
    // detects the final T curve
    if(exitTimeout >0) {
      setRightMotorAVoltage(-100);
      setLeftMotorAVoltage(180);
      exitTimeout--;
    }
    if (sensor_state == 0){
      flagOtherPath = false;
      exitTimeout = 10;
    } else {
      // PID for the U curve
      int u = pid(offset, DT, false);
      setRightMotorAVoltage(- (speed - u));
      setLeftMotorAVoltage(speed + u );
    }
  } else {
    // no obstacle
    offset = RGBLineFollower.getPositionOffset();
    
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

    int u = pid(offset, DT, flagCurve);

    setRightMotorAVoltage(- (speed - u));
    setLeftMotorAVoltage(speed + u );
  }

  if (flagUltraSensor == 2){
    // checks for obstacle every 3 cycles
    int distance = ultraSensor.distanceCm();
    if (distance <= 27){
      flagObstacle = true;
    } else {
      flagObstacle = false;
    }
    flagUltraSensor = 0;

  } else {
    flagUltraSensor++;
  }

  // debug if there is an obstacle
  digitalWrite(LEDPIN, flagOtherPath);
}


void waitNextPeriod()
{
  static long LastMillis = 0;
  long timeToWait = DT - (millis() - LastMillis);
  if (timeToWait > 0)
    delay(timeToWait);
  LastMillis = millis();
}

