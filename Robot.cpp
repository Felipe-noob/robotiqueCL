#include "Robot.h"

const int first_line = 560;
const int second_line = 0;
const int third_line = second_line;
const int line_after_first = 0;
const int line_after_second = 0;

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
      // State transition to Obstacle found
      if (obstacleAhead) {
        nextState = OBSTACLEFOUND;
      } else if (distance > 560 && !curveTimeout && !curveCooldown){
        nextState = CURVE;
        curveTimeout = 8;
        curveCooldown = 70;
      } else {
        // Not a curve or still on cooldown
        nextState = STRAIGHT;
        // Substracts cooldown, but sets a limit to 0 to avoid overflow. This is also possible using integers that support more bits, but it is still less safe
        if(--curveCooldown < 0) curveCooldown = 0;
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
      digitalWrite(LEDPIN, HIGH);

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
      int u = pid(offset, DT, false);

      // Declaration of SPEED here equals to BASE_SPEED to be able to modify later ( we want to go faster in a straight line, so just modify the speed variable here).
      // Also, by declaring speed here, instead of globally, we limit its scope and any optimization the compiler might try doing.
      int speed = BASE_SPEED;
      setRightMotorAVoltage(- (speed - u));
      setLeftMotorAVoltage(speed + u );
      break;
    }
    
    case CURVE: {
      int u = pid(offset, DT, true);
      // Special speed at curve
      int speed = CURVE_SPEED;
      setRightMotorAVoltage(- (speed - u));
      setLeftMotorAVoltage(speed + u );
      break;
    }

    case OBSTACLEFOUND: {
      setRightMotorAVoltage(0);
      setLeftMotorAVoltage(110);
      break;
    }

    case PATHOBSTACLE: {
      // When in alternate path, robot goes slower
      int speed = 0.75 * BASE_SPEED;
      int u = pid(offset, DT, false);
      setRightMotorAVoltage(- (speed - u));
      setLeftMotorAVoltage(speed + u ); 
      break;
    }

    case RESUMECOURSE: {
      // Turns violently
      setRightMotorAVoltage(0);
      setLeftMotorAVoltage(140);
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