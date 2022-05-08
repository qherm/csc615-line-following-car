// /**************************************************************
// * Class: CSC-615-01 Spring 2022
// * Name: Leiyi Gao / Shane Waxler
// * Student ID: 920871013
// * Github ID: gaolarry1987
// * Project: <project name> like Assignment 5- Follow in Line but Stop
// *
// * File: thread.c
// *
// * Description: Threading for the sensors for the line following
// *              car.
// *
// **************************************************************/
// #include "thread.h"



// void *TRSensor(void *arg){//White or black line
// 	while(1){
// 		delay(100);
//     if(digitalRead(TRR) == HIGH){
// 			printf("The TRCT5000sensor detects white color line\n");
// 		}
// 		else{
// 			printf("The sensor detects black color line.\n");
// 		}
// 	}
	
//   return NULL;
// }

// // void *IRInffarred(void *arg){//go or stop
// //   while(1){
// //     delay(100);
// //     if(digitalRead(IRR) == LOW){
// //       printf("Stop\n");
// //     }
// //     else{
// //       printf("Go\n");
// //     }
// //   }
// // }
 
// int main(void)
// {
//   if(wiringPiSetup() == -1){ 
//     printf(" Wiripin initialization failed!");
//     return 1;
//   }
//   pinMode(TRR, INPUT);
//   pinMode(IRR, INPUT);
  
//   int thread1;
//   int thread2;
  
//   pthread_t newthread1;
//   pthread_t newthread2;
//   thread1 = pthread_create(&newthread1, NULL, TRSensor, NULL);
//   thread2 = pthread_create(&newthread2, NULL, IRInffarred, NULL);
//   pthread_join(newthread1,NULL);
//   pthread_join(newthread2,NULL);
  
//   return 0;
// }

/**************************************************************
* Class: CSC-615-01 Spring 2020
* Name: Shane Waxler
* Student ID: 918415347
* Github ID: ShaneWaxler1
* Project: Assignment 5 - Traffic 
*
* File: assignment5.c
*
* Description: Threaded line sensor and obstacle sensor read.
*
**************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <pthread.h>

#define PINOA 24 /* Obstacle Avoidance IR GPIO pin in */
#define PINLS 23 /* Line Sensor GPIO pin in */

typedef struct sensor 
{
	int pin;
	volatile int read;
} sensor;

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
