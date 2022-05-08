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
#ifndef __RUN_MOTOR_H__
#define __RUN_MOTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "DEV_Config.h"
#include <time.h>
#include "PCA9685.h"

// #define BUTTON_PIN 23
// #define L_MOTOR 0x40
// #define R_MOTOR 0x5f
// #define PWMB 5		// Change eventually: PCA_CHANNEL_5
// #define BIN1 3		// Change eventually: PCA_CHANNEL_3
// #define BIN2 4		// Change eventually: PCA_CHANNEL_4

void stop_motor();
void init_motors();
void run_motor(UBYTE motor, int dir, int speed);
void handler(int signo);

#endif 
