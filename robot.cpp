#include "Robot.h"


Robot::Robot(MeRGBLineFollower *lineFollower, MeUltrasonicSensor *obstacleSensor, int samplingTime){
  // Initial state Straight
  currState = STRAIGHT;
  offset = 0;
  RGBLineFollower = lineFollower;
  FrontObstacleSensor = obstacleSensor;
  DT = samplingTime;
  curveTimeout = 0;
  curveCooldown = 0;
}

void Robot::init(float kp){
  InitMotors();
  InitEncoders();

  RGBLineFollower->begin();

  // Setting line following sensitivity, which is used for adjusting line following response speed. The larger the value is,more sensitive it turns.
  RGBLineFollower->setKp(kp);
}

// Handles state transition.
int Robot::stateTransition(){
  prevState = currState;

  switch(currState){

    case STRAIGHT: 
      // State transition to Obstacle found
      if (checkObstacle() == 1) nextState = OBSTACLEFOUND;

      // State transition to Curve
      else if (abs(offset) > 270 && !curveTimeout && curveCooldown <= 0){
        nextState = CURVE;
        curveTimeout = 8;
        curveCooldown = 70;
      }
      else {
        nextState = STRAIGHT;
        curveTimeout--;
        }
      break;
    

    case CURVE:
      if (curveTimeout > 0){
        curveTimeout--;
        curveCooldown--;
        nextState = CURVE;
      }
    }
}

void Robot::routine(){
  RGBLineFollower->loop();


  offset = RGBLineFollower->getPositionOffset();
  switch(currState){
    case STRAIGHT: {
      int u = pid(offset, DT, false);
      setRightMotorAVoltage(- (speed - u));
      setLeftMotorAVoltage(speed + u );
      break;
    }
    case CURVE: {
      int u = pid(offset, DT, true);
      setRightMotorAVoltage(- (speed - u));
      setLeftMotorAVoltage(speed + u );
      break;
    }
  }

}