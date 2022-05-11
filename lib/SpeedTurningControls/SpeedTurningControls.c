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

double wheelSpeedRatio = 1;// Left / Right

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
function to be threaded so that the wheel speeds are accurate
*/
void balanceWheelSpeeds(){}


void setSpeed(int speed){
  if(wheelSpeedRatio == 1){
    Motor_Run(wheels[LEFT].motorID, wheels[LEFT].direction, speed);
    Motor_Run(wheels[RIGHT].motorID, wheels[RIGHT].direction, speed);
  }
  else if(wheelSpeedRatio > 1){
    Motor_Run(wheels[LEFT].motorID, wheels[LEFT].direction, speed);
    Motor_Run(wheels[RIGHT].motorID, wheels[RIGHT].direction, (1 / wheelSpeedRatio) * speed);
  }
  else if(wheelSpeedRatio < 1){
    Motor_Run(wheels[LEFT].motorID, wheels[LEFT].direction, wheelSpeedRatio * speed);
    Motor_Run(wheels[RIGHT].motorID, wheels[RIGHT].direction, speed);
  }
  setSpeed = speed;
}


void forward(){
  wheels[LEFT].direction = FORWARD;
  wheels[RIGHT].direction = FORWARD;
  Motor_Run(wheels[LEFT].motorID, wheels[LEFT].direction, setSpeed);
  Motor_Run(wheels[RIGHT].motorID, wheels[RIGHT].direction, setSpeed);
}

void reverse(){
  wheels[LEFT].direction = BACKWARD;
  wheels[RIGHT].direction = BACKWARD;
  Motor_Run(wheels[LEFT].motorID, wheels[LEFT].direction, setSpeed);
  Motor_Run(wheels[RIGHT].motorID, wheels[RIGHT].direction, setSpeed);
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
    scale = ((pivot - (wheelToWheelWidth / 2)) / (pivot + (wheelToWheelWidth / 2)));
    wheelSpeedRatio = 1 / scale;
  }
  else if(pivot < 0){
    insideWheel = LEFT;
    scale = ((pivot + (wheelToWheelWidth / 2)) / (pivot - (wheelToWheelWidth / 2)));
    wheelSpeedRatio = scale;
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
  Motor_Run(wheels.[insideWheel].motorID, wheels[insideWheel].direction, setSpeed * scale);
}

void centerPointTurn(int spinDirection){
  int forwardWheel = LEFT;
  if(spinDirection == LEFT){
    forwardWheel = RIGHT;
  }
  wheels[spinDirection].direction = BACKWARD;
  wheels[forwardWheel].direction = FORWARD;
  Motor_Run(wheels[forwardWheel].motorID, wheels[forwardWheel].direction, setSpeed);
  Motor_Run(wheels[spinDirection].motorID, wheels[spinDirection].direction, setSpeed);
}

void wheelPivotTurn(int direction){
  if(direction == RIGHT){
    turnAroundPivot(wheelToWheelWidth / 2);
  }
  else{
    turnAroundPivot( wheelToWheelWidth / -2);
  }
}