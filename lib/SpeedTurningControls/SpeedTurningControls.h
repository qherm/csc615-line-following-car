/***********************************************************
* Class: CSC-615-01 Spring 2022
* Name: Francis Ranallo
* Student ID: 920830414
* GitHub ID: Franallo
* Project: line following car project - Autobots group
*
* File: SpeedTurningControls.h
*
* Description: This file contains function definitions to change the speed of the car
*               as well as turning, and balances the speed of the two wheels
***********************************************************/
#ifndef SpeedTurningControls_H
#define SpeedTurningControls_H

#include "ls7336r.c"
#include "MotorDriver.h"

#define LEFT 0
#define RIGHT 1

#define FORWARD 1
#define BACKWARD -1

struct Wheel {
  int chipEnable;
  int motorId;
  int direction;
  int lastReadTime;
  int lastCount;
  int calcSpeed;
} wheels[2];

wheels[LEFT].chipEnable = SPI0_CE1;//check for proper wheels
wheels[LEFT].motorID = MOTORB;
wheels[LEFT].direction = FORWARD;

wheels[RIGHT].chipEnable = SPI0_CE0;//check for proper wheels
wheels[RIGHT].motorID = MOTORA;
wheels[RIGHT].direction = FORWARD;

double wheelRadius = 4;//cm //check for proper lengths
double wheelToWheelWidth = 20;//cm

void initControls();

void balanceWheelSpeeds();

void setSpeed(int speed);

void forward();
void reverse();
void stop();

void turnAroundPivot(double pivot);
void centerPointTurn(int spinDirection);
void wheelPivotTurn(int direction);

#endif