#pragma once

#include "MeRGBLineFollower.h"
#include "Motors.h"
#include "Encoders.h"

enum RobotState {
  STRAIGHT,
  CURVE,
  OBSTACLE,
  CLEAREDOBSTACLE
};


class Robot {
  
  public:
  RobotState currState;
  RobotState nextState;
  RobotState prevState;
  MeRGBLineFollower &RGBLineFollower;

  Robot(MeRGBLineFollower &lineFollower);

  /*
  Initializes Motors and Encoders
  */
  void init();

  void routine();

  int changeState(RobotState target);

};