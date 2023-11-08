#pragma once
#include <Arduino.h>
#include "CentraleUltrasonicSensor.h"
#include "MeRGBLineFollower.h"
#include "Motors.h"
#include "Encoders.h"
#include "pid.h"

#define LEDPIN 2
#define STRAIGHTSPEED 140
#define WAITINGCURVESPEED 60
#define CURVESPEED 40
#define BASESPEED 50

enum RobotState {
  STRAIGHT,
  WAITINGCURVE,
  CURVE,
  AFTERCURVE,
  OBSTACLEFOUND,
  PATHOBSTACLE,
  RESUMECOURSE
};


class Robot {
  private:
  RobotState currState, nextState, prevState;
  int DT, curveTimeout, curveCooldown, obstacleCooldown, exitTimeout, leftEncoder, rightEncoder, curveCount;
  float averageOffset;
  bool obstacleAhead;
    
  public:
  MeRGBLineFollower *RGBLineFollower;
  CentraleUltrasonicSensor *FrontObstacleSensor;
  int16_t offset;

  Robot(MeRGBLineFollower *lineFollower, CentraleUltrasonicSensor *obstacleSensor, int samplingTime);

  /*
  * Initializes Motors, Encoders and sets up the RGB Line Follower
  */
  void init();

  /*
  * Checks wether it has obstacle or not. Also counts as 
  */
  void checkObstacle();

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
  * Prints stuff
  */
  void printInfo();
};