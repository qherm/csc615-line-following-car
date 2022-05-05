/**************************************************************
* Class: CSC-615-01 Spring 2022
* Name: Leiyi Gao
* Student ID: 920871013
* Github ID: gaolarry1987
* Project: CSC615 Final Car Project
*
* File: thread.h
*
* Description: Header file for the sensor threading.
*
**************************************************************/
#include <wiringPi.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define TRL 0   // TR Sensor Left
#define TRR 1
#define TRM 2
#define IRL 3   // IR Sensor Left
#define IRR 4
#define IRM 5

void *TRSensor(void *arg);
void *IRInffarred(void *arg);