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
#include <pigpio.h>

#define LEFT 0
#define RIGHT 1

#define FORWARD 1
#define BACKWARD -1

struct Wheel {
  int chipEnable;
  int motorId;
  int direction;
  uint32_t lastReadTime;//tick of read time, microseconds
  int lastCount;//revolutions at read time
  double calcSpeed;//revolutions per second
  int speedAdjust;
} wheels[2], default = {0,0,FORWARD,0,0,0,0};

wheels[LEFT] = default;
wheels[RIGHT] = default;

wheels[LEFT].chipEnable = SPI0_CE1;//check for proper wheels
wheels[LEFT].motorID = MOTORB;

wheels[RIGHT].chipEnable = SPI0_CE0;//check for proper wheels
wheels[RIGHT].motorID = MOTORA;

double wheelRadius = 4;//cm //check for proper lengths
double wheelToWheelWidth = 20;//cm

void initControls();

void* calculateInstantSpeed(void*);
void stopCalcInstantSpeed();
void* balanceWheelSpeeds(void*);
void stopBalanceWheelSpeeds();

double getInstantRPS(int wheel);//returns the speed in revolutions per second of the parameterized wheel
double getInstantCMPS(int wheel);//returns the speed in centimeters per second of the parameterized wheel

void setSpeed(int speed);

void forward();//straight forward
void reverse();//straight backward
void stop();

void turnAroundPivot(double pivot);
void centerPointTurn(int spinDirection);//spin around the center point, staying in place
void wheelPivotTurn(int direction);//turn where the parameterized wheel stops turning while allowing the other to continue

#endif