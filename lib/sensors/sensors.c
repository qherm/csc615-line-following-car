/**************************************************************
* Class: CSC-615-01 Spring 2022
* Name: Leiyi Gao / Shane Waxler
* Student ID: 920871013
* Github ID: gaolarry1987
* Project: <project name> like Assignment 5- Follow in Line but Stop
*
* File: thread.c
*
* Description: Threading for the sensors for the line following
*              car.
*
**************************************************************/
#include "thread.h"



void *TRSensor(void *arg){//White or black line
	while(1){
		delay(100);
    if(digitalRead(TRR) == HIGH){
			printf("The TRCT5000sensor detects white color line\n");
		}
		else{
			printf("The sensor detects black color line.\n");
		}
	}
	
  return NULL;
}

// void *IRInffarred(void *arg){//go or stop
//   while(1){
//     delay(100);
//     if(digitalRead(IRR) == LOW){
//       printf("Stop\n");
//     }
//     else{
//       printf("Go\n");
//     }
//   }
// }
 
int main(void)
{
  if(wiringPiSetup() == -1){ 
    printf(" Wiripin initialization failed!");
    return 1;
  }
  pinMode(TRR, INPUT);
  pinMode(IRR, INPUT);
  
  int thread1;
  int thread2;
  
  pthread_t newthread1;
  pthread_t newthread2;
  thread1 = pthread_create(&newthread1, NULL, TRSensor, NULL);
  thread2 = pthread_create(&newthread2, NULL, IRInffarred, NULL);
  pthread_join(newthread1,NULL);
  pthread_join(newthread2,NULL);
  
  return 0;
}
