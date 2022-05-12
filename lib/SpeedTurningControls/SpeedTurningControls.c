/***********************************************************
* Class: CSC-615-01 Spring 2022
* Name: Francis Ranallo
* Student ID: 920830414
* GitHub ID: Franallo
* Project: line following car project - Autobots group
*
* File: SpeedTurningControls.c
*
* Description: This file contains function definitions to change the speed of the car
*               as well as turning, and balances the speed of the two wheels
***********************************************************/

#include "SpeedTurningControls.h"

int setSpeed = 0;// speed that is set by us

double wheelTurnSpeedRatio = 1;// Left / Right

bool calcSpeed = true;
bool balanceSpeed = true;

int initControls(){
  Motor_Init();  
  if(initLS7336RChip(wheels[LEFT].chipEnable) != 0){
    return -1;
  }
  if(initLS7336RChip(wheels[RIGHT].chipEnable) != 0){
    return -1;
  }
  return 0;
}

/*
function to be threaded so we know the instantaneous speed of the wheels
*/
void* calculateInstantSpeed(void*){//gpio must be initialized
  uint32_t readTime;
  int readCount;
  calcSpeed = true;
  while(calcSpeed){
    for(int i = 0; i < 2; i++){
      readCount = readLS7336RCounter(wheels[i].chipEnable);
      readTime = gpioTick();
      wheels[i].calcSpeed = ((wheels[i].lastCount - readCount) / ((wheels[i].lastReadTime - readTime) / 1000000));//revolutions per second
      wheels[i].lastCount = readCount;
      wheels[i].lastReadTime = readTime;
      sleep(.5);
    }
  }
}
void stopCalcInstantSpeed(){
  calcSpeed = false;
}

/*
function to be threaded so that the wheel speeds are accurate
*/
void* balanceWheelSpeeds(void*){
  balanceSpeed = true;
  double speedRatio = 1;
  double acceptableError = .3;//this value could change with testing
  int speed = 0;
  while(balanceSpeed){
    if(wheelTurnSpeedRatio == 1){//wheel speed should be equal
      speedRatio = (wheels[LEFT].calcSpeed / wheels[RIGHT].calcSpeed);
      speed = setSpeed;
    }
    else if(wheelTurnSpeedRatio > 1){//left wheel should be faster, turning right
      speedRatio = (wheels[LEFT].calcSpeed / (wheelTurnSpeedRatio * wheels[RIGHT].calcSpeed));
      speed = (( 1 / wheelTurnSpeedRatio) * setSpeed);
    }
    else{//right wheel should be faster, turning left
      speedRatio = ((wheels[LEFT].calcSpeed * ( 1 / wheelTurnSpeedRatio)) / wheels[RIGHT].calcSpeed);
      speed = (wheelTurnSpeedRatio * setSpeed);
    }

    int wheel = -1;
    if(speedRatio < 1 - acceptableError){//left is going too slow
      if(wheels[LEFT].speedAdjust < 0){
        wheels[LEFT].speedAdjust += 1;
        wheel = LEFT;
      }
      else{
        wheels[RIGHT].speedAdjust -= 1;
        wheel = RIGHT;
      }
    }
    else if(speedRatio > 1 + acceptableError){//right is going too slow
      if(wheels[RIGHT].speedAdjust < 0){
        wheels[RIGHT].speedAdjust += 1;
        wheel = RIGHT;
      }
      else{
        wheels[LEFT].speedAdjust -= 1;
        wheel = LEFT;
      }
    }

    if(wheel != -1){
      Motor_Run(wheels[wheel].motorID, wheels[wheel].direction, speed + wheels[wheel].speedAdjust)
    }
    sleep(1);
  }
}
void stopBalanceWheelSpeeds(){
  balanceSpeed = false;
}


double getInstantRPS(int wheel){
  return wheels[wheel].calcSpeed;
}
double getInstantCMPS(int wheel){
  return (wheels[wheel].calcSpeed * 2 * 3.1415926 * wheelRadius);
}


void setSpeed(int speed){
  if(wheelTurnSpeedRatio == 1){
    Motor_Run(wheels[LEFT].motorID, wheels[LEFT].direction, speed + wheels[LEFT].speedAdjust);
    Motor_Run(wheels[RIGHT].motorID, wheels[RIGHT].direction, speed + wheels[RIGHT].speedAdjust);
  }
  else if(wheelTurnSpeedRatio > 1){
    Motor_Run(wheels[LEFT].motorID, wheels[LEFT].direction, speed + wheels[LEFT].speedAdjust);
    Motor_Run(wheels[RIGHT].motorID, wheels[RIGHT].direction, (1 / wheelTurnSpeedRatio) * speed + wheels[RIGHT].speedAdjust);
  }
  else if(wheelTurnSpeedRatio < 1){
    Motor_Run(wheels[LEFT].motorID, wheels[LEFT].direction, wheelTurnSpeedRatio * speed + wheels[LEFT].speedAdjust);
    Motor_Run(wheels[RIGHT].motorID, wheels[RIGHT].direction, speed + wheels[RIGHT].speedAdjust);
  }
  setSpeed = speed;
}


void forward(){
  wheels[LEFT].direction = FORWARD;
  wheels[RIGHT].direction = FORWARD;
  Motor_Run(wheels[LEFT].motorID, wheels[LEFT].direction, setSpeed + wheels[LEFT].speedAdjust);
  Motor_Run(wheels[RIGHT].motorID, wheels[RIGHT].direction, setSpeed + wheels[RIGHT].speedAdjust);
}

void reverse(){
  wheels[LEFT].direction = BACKWARD;
  wheels[RIGHT].direction = BACKWARD;
  Motor_Run(wheels[LEFT].motorID, wheels[LEFT].direction, setSpeed + wheels[LEFT].speedAdjust);
  Motor_Run(wheels[RIGHT].motorID, wheels[RIGHT].direction, setSpeed + wheels[RIGHT].speedAdjust);
}

void stop(){
  Motor_Stop(wheels[LEFT].motorID);
  Motor_Stop(wheels[RIGHT].motorID);
  setSpeed = 0;
}

/*
function that changes the speed of the wheels to make the car turn.
the turn is centered around a pivot point somewhere on the wheels' axis,
with 0 being at the midpoint between the two wheels
*/
void turnAroundPivot(double pivot){
  int insideWheel, outsideWheel, scale;
  if(pivot > 0){
    insideWheel = RIGHT;
    outsideWheel = LEFT;
    scale = ((pivot - (wheelToWheelWidth / 2)) / (pivot + (wheelToWheelWidth / 2)));
    wheelTurnSpeedRatio = 1 / scale;
  }
  else if(pivot < 0){
    insideWheel = LEFT;
    outsideWheel = RIGHT;
    scale = ((pivot + (wheelToWheelWidth / 2)) / (pivot - (wheelToWheelWidth / 2)));
    wheelTurnSpeedRatio = scale;
  }
  else if(pivot == 0){
    centerPointTurn(RIGHT);
  }

  if(scale < 0){
    wheels[insideWheel].direction = wheels[outsideWheel].direction * -1;
  }
  else{
    wheels[insideWheel].direction = wheels[outsideWheel].direction;
  }
  Motor_Run(wheels.[insideWheel].motorID, wheels[insideWheel].direction, setSpeed * scale + wheels[insideWheel].speedAdjust);
}

void centerPointTurn(int spinDirection){
  int forwardWheel = LEFT;
  if(spinDirection == LEFT){
    forwardWheel = RIGHT;
  }
  wheels[spinDirection].direction = BACKWARD;
  wheels[forwardWheel].direction = FORWARD;
  Motor_Run(wheels[forwardWheel].motorID, wheels[forwardWheel].direction, setSpeed + wheels[forwardWheel].speedAdjust);
  Motor_Run(wheels[spinDirection].motorID, wheels[spinDirection].direction, setSpeed + wheels[spinDirection].speedAdjust);
}

void wheelPivotTurn(int direction){
  if(direction == RIGHT){
    turnAroundPivot(wheelToWheelWidth / 2);
  }
  else{
    turnAroundPivot( wheelToWheelWidth / -2);
  }
}