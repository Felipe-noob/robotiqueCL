#include "Robot.h"

Robot::Robot(MeRGBLineFollower *lineFollower, CentraleUltrasonicSensor *obstacleSensor, int samplingTime){
  // Initial state Straight
  currState = STRAIGHT;
  offset = 0;
  RGBLineFollower = lineFollower;
  FrontObstacleSensor = obstacleSensor;
  DT = samplingTime;
  curveTimeout = 0;
  curveCooldown = 0;
  leftEncoder = 0;
  rightEncoder = 0;
  curveCount = 0;
}

void Robot::init(){
  InitMotors();
  InitEncoders();

  RGBLineFollower->begin();

  // Setting line following sensitivity, which is used for adjusting line following response speed. The larger the value is,more sensitive it turns.
  RGBLineFollower->setKp(1);
}

// Handles state transition.
void Robot::stateTransition(){

  switch(currState){

    case STRAIGHT:{
      // State transition to Curve
      // Checks if offset is high enough and if the conditions to transition to CURVE state are set
      const int distance = getPosition1() - leftEncoder;
      const int lineIndex = curveCount % 4;
      bool endOfLine =  false;

      switch (lineIndex) {
        case 0: 
          endOfLine = distance > 100;
          break;
        case 1:
          endOfLine = distance > 0;
          break;
        case 2:
          endOfLine = distance > 200;
          break;
        case 3:
          endOfLine = distance > 560;
          break;
        default:
          endOfLine = true;
          break;
      }
      // State transition to Obstacle found
      if (obstacleAhead) {
        nextState = OBSTACLEFOUND;
      } else if (endOfLine){
        nextState = WAITINGCURVE;
        
      } else {
        // Not a curve or still on cooldown
        nextState = STRAIGHT;
        // Substracts cooldown, but sets a limit to 0 to avoid overflow. This is also possible using integers that support more bits, but it is still less safe
        if(--curveCooldown < 0) curveCooldown = 0;
      }
      break;
    }
    case WAITINGCURVE: {
      if (abs(offset) > 270 && !curveTimeout && !curveCooldown) {
        curveTimeout = 6;
        curveCooldown = 50;
        nextState = CURVE;
      } else {
        nextState = WAITINGCURVE;
      }
      break;
    }

    case CURVE:
      if (curveTimeout){
        // keeps on curve until timeout ends
        curveTimeout--;
        curveCooldown--;
        nextState = CURVE;
      } else {
        leftEncoder = getPosition1();
        rightEncoder = getPosition2();
        curveCount++;
        nextState = STRAIGHT;
      }
      break;

    
    case OBSTACLEFOUND:
      // Until the detector stops detecting an obstacle, the robot will stay in this state, making the slight curve.
      if (obstacleAhead) nextState = OBSTACLEFOUND;
      else {
        nextState = PATHOBSTACLE;
        obstacleCooldown = 40;
      }

      break;

    case PATHOBSTACLE:
      // const int curveIndex = (getPosition1() - leftTurns) - (getPosition2() - rightTurns);
      if (offset > 160 && obstacleCooldown <= 0){
        nextState = RESUMECOURSE;
        exitTimeout = 8;
      } else {
        obstacleCooldown--;
        nextState = PATHOBSTACLE;
      }
      break;

    case RESUMECOURSE:
      if(exitTimeout){
        exitTimeout--;
        nextState = RESUMECOURSE;
      }else nextState = STRAIGHT;
      break;

    } // end switch STATE TRANSITION

  prevState = currState;
  currState = nextState;   
}

void Robot::routine(){
  RGBLineFollower->loop();
  checkObstacle();
  offset = RGBLineFollower->getPositionOffset();

  switch(currState){
    case STRAIGHT: {
      const int u = pid(offset, DT, 0.1, 0.4, 0.1, 0);
      const int speed = 65;
      setRightMotorAVoltage(- (speed - u));
      setLeftMotorAVoltage(speed + u );
      break;
    }
    
    case WAITINGCURVE: {
      int u = pid(offset, DT, 0.1, 0.4, 0.1, 0);

      int speed = 50;
      setRightMotorAVoltage(- (speed - u));
      setLeftMotorAVoltage(speed + u );
      break;
    }

    case CURVE: {
      int u = pid(offset, DT, 0.4, 0.5, 0.1, 0.1);
      // Special speed at curve
      int speed = 40;
      setRightMotorAVoltage(- (speed - u));
      setLeftMotorAVoltage(speed + u );
      break;
    }

    case OBSTACLEFOUND: {
      setRightMotorAVoltage(-110);
      setLeftMotorAVoltage(0);
      break;
    }

    case PATHOBSTACLE: {
      // When in alternate path, robot goes slower
      int speed = 38;
      int u = pid(offset, DT, 0.1, 0.4, 0.1, 0);
      setRightMotorAVoltage(- (speed - u));
      setLeftMotorAVoltage(speed + u ); 
      break;
    }

    case RESUMECOURSE: {
      // Turns violently
      setRightMotorAVoltage(-140);
      setLeftMotorAVoltage(0);
      break;
    }
  } // end switch
  stateTransition();
}


void Robot::checkObstacle(){
  static int activateSensor;
  if (activateSensor == 2){
    int distance = FrontObstacleSensor->distanceCm();
    if (distance <= 24) obstacleAhead = true;
    else obstacleAhead = false;
    activateSensor = 0;
  } else activateSensor++;
}


void Robot::printInfo(){
  Serial.print(currState);
  Serial.print(",");
  Serial.println(offset);
}