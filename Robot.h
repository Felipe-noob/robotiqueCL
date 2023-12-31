#pragma once
#include <Arduino.h>
#include "CentraleUltrasonicSensor.h"
#include "MeRGBLineFollower.h"
#include "Motors.h"
#include "Encoders.h"
#include "pid.h"

#define LEDPIN 2
#define STRAIGHTSPEED 130
#define WAITINGCURVESPEED 60
#define CURVESPEED 40
#define BASESPEED 50

enum RobotState {
  INITIALSTATE,
  STRAIGHT,
  WAITINGCURVE,
  CURVE,
  AFTERCURVE,
  OBSTACLEFOUND,
  TRANSITIONPATHCURVE,
  PATHOBSTACLE,
  STABILIZE,
  VOITUREFOUND
};


class Robot {
  private:
  RobotState currState, nextState, prevState;
  int DT, curveTimeout, curveCooldown, obstacleCooldown, exitTimeout, leftEncoder, rightEncoder, curveCount;
  float averageOffset;
  bool obstacleAhead, carOnTheRight;
    
  public:
  MeRGBLineFollower *RGBLineFollower;
  CentraleUltrasonicSensor *FrontObstacleSensor, *RightCarSensor;
  int16_t offset;

  Robot(MeRGBLineFollower *lineFollower, CentraleUltrasonicSensor *obstacleSensor, CentraleUltrasonicSensor *carSensor, int samplingTime);

  /*
  * Initializes Motors, Encoders and sets up the RGB Line Follower
  */
  void init();

  /*
  * Checks wether it has obstacle or not. Also counts as 
  */
  void checkObstacle();


  /*
  * Checks wether it has voiture or not. Also counts as 
  */
  void checkVoiture();

  /*
  * Main routine. Robot acts accordingly with its states.
  * 
  */
  void routine();


  /* 
  * Handles state transition at every cycle
  */
  void stateTransition();


  /*
  * Helps determine if the robot has stabilized
  */
  void movingAverage();

  /*
  * Turns 90 degrees to the right
  */
  void turnRight();

  void goStraight(int decoderNumber, int speed);

  /*
  * Prints stuff
  */
  void printInfo();
};