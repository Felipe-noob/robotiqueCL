#include "Robot.h"


Robot::Robot(MeRGBLineFollower &lineFollower){
  currState = STRAIGHT;
  RGBLineFollower = lineFollower;
}

void Robot::init(float kp)){
  InitMotors();
  InitEncoders();

  RGBLineFollower.begin();

  // Setting line following sensitivity, which is used for adjusting line following response speed. The larger the value is,more sensitive it turns.
  RGBLineFollower.setKp(kp);
}

int Robot::changeState(RobotState target){

}

void Robot::routine(){
  RGBLineFollower.
}