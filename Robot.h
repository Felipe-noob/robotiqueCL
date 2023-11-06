#pragma once
#include <Arduino.h>
#include "CentraleUltrasonicSensor.h"
#include "MeRGBLineFollower.h"
#include "Motors.h"
#include "Encoders.h"
#include "pid.h"

#define BASE_SPEED 50
#define CURVE_SPEED 40
#define LEDPIN 2


enum RobotState {
  STRAIGHT,
  CURVE,
  OBSTACLEFOUND,
  PATHOBSTACLE,
  RESUMECOURSE
};


class Robot {
  private:
  RobotState currState, nextState, prevState;
  int DT, curveTimeout, curveCooldown, obstacleCooldown, exitTimeout, leftEncoder, rightEncoder;
  bool obstacleAhead;
    
  public:
  MeRGBLineFollower *RGBLineFollower;
  CentraleUltrasonicSensor *FrontObstacleSensor;
  int16_t offset;

  Robot(MeRGBLineFollower *lineFollower, CentraleUltrasonicSensor *obstacleSensor, int samplingTime);

  /*
  * Initializes Motors, Encoders and sets up the RGB Line Follower
  */
  void init(float kp);

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
  * Prints stuff
  */
  void printInfo();
};