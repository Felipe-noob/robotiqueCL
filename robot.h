#pragma once
#include <MeUltrasonicSensor.h>
#include "MeRGBLineFollower.h"
#include "Motors.h"
#include "Encoders.h"
#include "pid.h"

#define BASE_SPEED 50



enum RobotState {
  STRAIGHT,
  CURVE,
  OBSTACLEFOUND,
  CLEAREDOBSTACLE
};


class Robot {
  private:
  RobotState currState;
  RobotState nextState;
  RobotState prevState;
  int DT, speed, curveTimeout, curveCooldown;
    
  public:
  MeRGBLineFollower *RGBLineFollower;
  MeUltrasonicSensor *FrontObstacleSensor;
  int16_t offset;

  Robot(MeRGBLineFollower *lineFollower, MeUltrasonicSensor *obstacleSensor, int samplingTime);

  /*
  * Initializes Motors, Encoders and sets up the RGB Line Follower
  */
  void init(float kp);

  /*
  * Checks wether it has obstacle or not. Also counts as 
  */
  int checkObstacle();

  /*
  * Main routine. Robot acts accordingly with its states.
  * 
  */
  void routine();


  /* 
  * Handles state transition at every cycle
  */
  int stateTransition();

};