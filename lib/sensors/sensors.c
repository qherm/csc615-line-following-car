/**************************************************************
* Class: CSC-615-01 Spring 2020
* Name: Shane Waxler
* Student ID: 918415347
* Github ID: ShaneWaxler1
* Project: Assignment 5 - Traffic 
*
* File: sensors.c
*
* Description: Threaded line sensor and obstacle sensor read.
*
**************************************************************/

#include "sensors.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <pigpio.h>
#include <pthread.h>

sensor* new_sensor(int pin){
	sensor* s = malloc(sizeof(sensor));
	s->pin = pin;
	s->read = 0;
	s->cont = true;
	return s;
}

void destroy_sensor(sensor* sensor){
	free(sensor);
}

void* sense(void* ptr){
	sensor* s = (struct sensor*) ptr;
	int pin = s->pin;
	printf("In thread. Pin number %d\n",pin);
	while(s->cont)
		s->read = gpioRead(pin);
	return NULL;
}
