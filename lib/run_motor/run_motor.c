/*********************************************************
* Class CSC-615-01 Spring 2022
* Name: Shane Waxler
* Student ID: 918415347
* Github ID: ShaneWaxler1
* Project: Assignment3 - Start Your Motors
*
* File: motordriver.c
*
* Description: Run a motor forwards and backwards using 
*	       a waveshare hat.
*
*********************************************************/
#include "run_motor.h"
#include <pigpio.h>
#include <unistd.h>

#define BUTTON_PIN 23
#define MOTOR_HEX 0x40
#define PWMB 5		// Change eventually: PCA_CHANNEL_5
#define BIN1 3		// Change eventually: PCA_CHANNEL_3
#define BIN2 4		// Change eventually: PCA_CHANNEL_4

void stop_motor()
{
	PCA9685_SetPwmDutyCycle(PWMB, 0);	
}

void init_motor()
{
	PCA9685_Init(0x40);
	PCA9685_SetPWMFreq(100);
}

void run_motor(int direction, int speed)
{
	if(speed > 100)
		speed = 100;

	PCA9685_SetPwmDutyCycle(PWMB, speed);
	PCA9685_SetLevel(BIN1, direction);
	PCA9685_SetLevel(BIN2, !direction);	
}

// void  handler(int signo)
// {
// 	printf("\r\nHandler:Motor Stop\r\n");	
// 	stop_motor();
// 	DEV_ModuleExit();
// 	gpioTerminate();
// 	exit(0);
// }

// int main(int argc, char *argv[])
// {
// 	int i;
	
// 	// Setup
// 	if(DEV_ModuleInit())
// 		exit(0);
	
// 	if(gpioInitialise() < 0)
// 	{
// 		fprintf(stderr, "pigpio initialize failure\n");
// 		return 1;
// 	}
	
// 	gpioSetMode(BUTTON_PIN, PI_INPUT);
// 	signal(SIGINT, handler);
	
// 	// Wait for button press	
// 	while(!gpioRead(BUTTON_PIN)){}
	
// 	init_motor();
	
// 	// Speed forward 100 -> 15
// 	run_motor(0,100);
// 	sleep(2);
// 	for(i = 95; i >=15; i-=5)
// 	{
// 		run_motor(0,i);
// 		sleep(1);
// 	}
	
// 	stop_motor();
// 	sleep(1);
	
// 	// Speed backward 15 -> 100
// 	run_motor(1,15);
// 	for(i = 20; i <= 100; i+=5)
// 	{
// 		run_motor(1,i);
// 		sleep(1);
// 	}

// 	sleep(2);
// 	stop_motor(PWMB);	
// 	DEV_ModuleExit();
// 	gpioTerminate();
// 	return 0;
// }
