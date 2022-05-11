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
#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <wiringPi.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

// Pin numbers using GPIO nums
#define TRL 0   // TR Sensor Left
#define TRR 1
#define TRM 2
#define IRL 24   // IR Sensor Left
#define IRR 18
#define IRM 23

typedef struct sensor 
{
	int pin;
	volatile int read;
	bool cont;
} sensor;

sensor* new_sensor(int pin);
void *sense(void *arg);
void destroy_sensor(sensor* sensor);

#endif
