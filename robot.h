#pragma once

#include "MeRGBLineFollower.h"
#include "Motors.h"
#include "Encoders.h"

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


  public:
  MeRGBLineFollower *RGBLineFollower;
  MeUltrasonicSensor *FrontObstacleSensor;
  int16_t offset;

  Robot(MeRGBLineFollower *lineFollower, MeUltrasonicSensor *obstacleSensor);

  /*
  * Initializes Motors, Encoders and sets up the RGB Line Follower
  */
  void init(float kp);


  /*
  * Main loop
  */
  void routine();

  int changeState(RobotState target);

};