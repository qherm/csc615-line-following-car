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

#include "thread.h"
#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <pthread.h>

#define PINOA 24 /* Obstacle Avoidance IR GPIO pin in */
#define PINLS 23 /* Line Sensor GPIO pin in */

void* sense(void* ptr)
{
	sensor* s = (struct sensor*) ptr;
	int pin = s->pin;
	while(1)
		s->read = gpioRead(pin);
}

// int main()
// {
// 	if(gpioInitialise() < 0)
// 	{
// 		fprintf(stderr, "pigpio initialise failure\n");
// 		return 1;
// 	}
	
// 	gpioSetMode(PINOA, PI_INPUT);
// 	gpioSetMode(PINLS, PI_INPUT);

// 	pthread_t obstacle_thread, line_thread;
// 	int obstacle_return, line_return;
	
// 	sensor obstacle, line;

// 	obstacle.pin = PINOA;
// 	obstacle.read = 0;

// 	line.pin = PINLS;
// 	line.read = 0;

// 	obstacle_return = pthread_create(&obstacle_thread, NULL, sense, &obstacle);
// 	line_return = pthread_create(&line_thread, NULL, sense, &line);
	
// 	while(1)
// 	{
// 		printf("Line read: %i\n", line.read);
// 		printf("Obstacle read: %i\n\n", obstacle.read);
// 		usleep(500000);
// 	}

// 	pthread_join(obstacle_thread, NULL);
// 	pthread_join(line_thread, NULL);

// 	printf("Obstacle thread returns: %i\n", obstacle_return);
// 	printf("Line thread returns: %i\n", line_return);

// 	return 0;
// }
