#include "Robot.h"

Robot::Robot(MeRGBLineFollower *lineFollower, CentraleUltrasonicSensor *obstacleSensor, CentraleUltrasonicSensor *carSensor, int samplingTime){
  // Initial state Straight
  currState = WAITINGCURVE;
  offset = 0;

  RGBLineFollower = lineFollower;
  FrontObstacleSensor = obstacleSensor;
  RightCarSensor = carSensor;

  DT = samplingTime;
  curveTimeout = 0;
  curveCooldown = 0;
  leftEncoder = 0;
  rightEncoder = 0;
  curveCount = 0;
  averageOffset = 0;

  carOnTheRight = 0;

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
          endOfLine = distance > 200;
          break;
        case 1:
          endOfLine = distance > 0;
          break;
        case 2:
          endOfLine = distance > 250;
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
      } else if (carOnTheRight){
        prevState = STRAIGHT;
        nextState = VOITUREFOUND;
      }
      else if (endOfLine){
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
      if(obstacleAhead) {
        curveCooldown--;
        nextState = OBSTACLEFOUND;
      } else if (carOnTheRight){
        prevState = WAITINGCURVE;
        nextState = VOITUREFOUND;
      }
      else if (averageOffset > 1400 && curveCooldown <= 0) {
        curveTimeout = 13;
        curveCooldown = 80;
        nextState = CURVE;
      } else {
        curveCooldown--;
        nextState = WAITINGCURVE;
      }
      break;
    }

    case CURVE:
      if(obstacleAhead) {
        curveCount++;
        curveTimeout--;
        curveCooldown--;
        nextState = OBSTACLEFOUND;
      } else if (carOnTheRight){
        prevState = CURVE;
        nextState = VOITUREFOUND;
      }
      else if (curveTimeout >= 0){
        // keeps on curve until timeout ends
        curveTimeout--;
        curveCooldown--;
        nextState = CURVE;
      } else {
        leftEncoder = getPosition1();
        rightEncoder = getPosition2();
        curveCount++;
        nextState = AFTERCURVE;
      }
      break;

    case AFTERCURVE: 
      curveCooldown--;
      if(obstacleAhead) {
        nextState = OBSTACLEFOUND;
      } else if (carOnTheRight){
        prevState = AFTERCURVE;
        nextState = VOITUREFOUND;
      }
      else if(averageOffset < 1000) {
        nextState = STRAIGHT;
      } else {
        nextState = AFTERCURVE;
      }
      break;
    
    case OBSTACLEFOUND: {
      if (obstacleAhead) nextState = OBSTACLEFOUND;
      else nextState = STABILIZE;
      
      // nextState = TRANSITIONPATHCURVE;
      break;
    }
    case TRANSITIONPATHCURVE: {
      static int timer;
      if (timer++ < 9) nextState = TRANSITIONPATHCURVE;
      else{
        timer = 0;
        averageOffset = 0;
        nextState = WAITINGCURVE;
      } 
      break;
    }

    case STABILIZE: {
      if (abs(offset >= 80)) nextState = STABILIZE;
      else {
        obstacleCooldown = 40;
        nextState = PATHOBSTACLE;
        }
      break;
    }

    case PATHOBSTACLE:
      // const int curveIndex = (getPosition1() - leftTurns) - (getPosition2() - rightTurns);
      if (offset < -160 && obstacleCooldown <= 0){
        nextState = TRANSITIONPATHCURVE;
        exitTimeout = 8;
      } else {
        obstacleCooldown--;
        nextState = PATHOBSTACLE;
      }
      break;

    
    case VOITUREFOUND:
      if (carOnTheRight) nextState = VOITUREFOUND;
      else {
        delay(2000);
        nextState = prevState;
        }
      break;
    } // end switch STATE TRANSITION

  
  currState = nextState;   
}

void Robot::routine(){

  RGBLineFollower->loop();
  checkObstacle();
  checkVoiture();
  offset = RGBLineFollower->getPositionOffset();
  movingAverage();

  switch(currState){
    case STRAIGHT: {
      const int u = pid(offset, DT, 0.01, 4, 1, 0);
      const int speed = STRAIGHTSPEED;
      setRightMotorAVoltage(speed - u);
      setLeftMotorAVoltage(speed + u);
      break;
    }
    
    case WAITINGCURVE: {
      const int u = pid(offset, DT, 0.01, 4, 1, 0.5);

      const int speed = WAITINGCURVESPEED;
      setRightMotorAVoltage(speed - u);
      setLeftMotorAVoltage(speed + u);
      break;
    }

    case CURVE: {
      const int u = pid(offset, DT, 0.03, 5, 1, 1);
      // Special speed at curve
      const int speed = CURVESPEED;
      setRightMotorAVoltage(speed - u);
      setLeftMotorAVoltage(speed + u);
      break;
    }

    case AFTERCURVE: {
      const int u = pid(offset, DT, 0.01, 4, 1, 0.5);
      const int speed = WAITINGCURVESPEED;
      setRightMotorAVoltage(speed - u);
      setLeftMotorAVoltage(speed + u);
      break;
    }
    case STABILIZE: {
      setLeftMotorAVoltage(-45);
      setRightMotorAVoltage(30);
      break;
    }
    case OBSTACLEFOUND: {
      setLeftMotorAVoltage(-50);
      setRightMotorAVoltage(30);
      // turnLeft();
      // turnRight();
      break;
    }
    case TRANSITIONPATHCURVE: {
      const int speed = 70;
      const int u = pid(offset, DT, 0.021, 4, 0.5, 0);
      setRightMotorAVoltage(speed - u + 30);
      setLeftMotorAVoltage(speed + u); 
      break;
    }
    case PATHOBSTACLE: {
      const int u = pid(offset, DT, 0.01, 4, 1, 1);
      const int speed = 50;
      setRightMotorAVoltage(speed -u );
      setLeftMotorAVoltage(speed +u );
      break;
    }

    case VOITUREFOUND: {
      setRightMotorAVoltage(0);
      setLeftMotorAVoltage(0);
    }
  } // end switch

  stateTransition();
}


void Robot::checkObstacle(){
  int distance = FrontObstacleSensor->distanceCm(100);
  if (distance <= 16) obstacleAhead = true;
  else obstacleAhead = false;
}

void Robot::checkVoiture(){
  int distance = RightCarSensor->distanceCm(90);
  if (distance <= 25) carOnTheRight = true;
  else carOnTheRight = false;
}

void Robot::movingAverage(){
  const float a = 0.9;
  averageOffset = (a*averageOffset + abs(offset));
}

void Robot::turnRight(){
  int initial = getPosition1();
  while(getPosition1() - initial < 161){
    setLeftMotorAVoltage(60);
    setRightMotorAVoltage(0);
  } 
  setLeftMotorAVoltage(0);
}

void Robot::turnLeft(){
  static int turns;
  for(int i = 0; i < 10 ; i++){
    setLeftMotorAVoltage(-40);
    setRightMotorAVoltage(30);
    delay(50);
  }
  

  turns = 0;
}

void Robot::goStraight(int decoderNumber, int speed){
  int currPos = getPosition1();
  while((getPosition1() - currPos) <= decoderNumber){
    setLeftMotorAVoltage(speed);
    setRightMotorAVoltage(speed);
  }
}

void Robot::printInfo(){
  // left,right
  
  Serial.print(getPosition1());
  Serial.print(",");
  Serial.print(getPosition2());
  Serial.print(",");
  Serial.print(currState);
  Serial.print(",");
  Serial.print(curveCount);
  Serial.print(",");
  Serial.print(offset);
  Serial.print(",");
  Serial.print(abs(offset));
  Serial.print(",");
  Serial.println(averageOffset);
}