#pragma once

enum RobotState {
  STRAIGHT,
  CURVE,
  OBSTACLE,
  CLEAREDOBSTACLE
};


class Robot {
  
  public:
  RobotState currState;
  Robot() : currState(STRAIGHT) {}
  int switchState(RobotState target);
};