/*********************************************************
* Class CSC-615-01 Spring 2022
* Name: Shane Waxler
* Student ID: 918415347
* Github ID: ShaneWaxler1
* Project: Final Project
*
* File: motordriver.h
*
* Description: Run a motor.
*
*********************************************************/
#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>      //printf()
#include <stdlib.h>     //exit()
#include <signal.h>

#include "DEV_Config.h"
#include <time.h>
#include "PCA9685.h"

void stop_motor();
void init_motor();
void run_motor(int direction, int speed);
void handler(int signo);

#endif 
