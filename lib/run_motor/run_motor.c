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
#include "MotorDriver.h"
#include <pigpio.h>
#include <unistd.h>

void stop_motor()
{
	PCA9685_SetPwmDutyCycle(PWMB, 0);	
}

void init_motors()
{
	PCA9685_Init(0x40);
	PCA9685_Init(0x5f);
	PCA9685_SetPWMFreq(100);
}

/*
* 	Author: Waveshare team. Rewrite this after testing.
*/
void run_motor(UBYTE motor, int dir, int speed)
{
	if(speed > 100)
        speed = 100;

    if(motor == MOTORA) {
        PCA9685_SetPwmDutyCycle(PWMA, speed);
        if(dir == FORWARD) {
            PCA9685_SetLevel(AIN1, 0);
            PCA9685_SetLevel(AIN2, 1);
        } else {
            PCA9685_SetLevel(AIN1, 1);
            PCA9685_SetLevel(AIN2, 0);
        }
    } else {
        PCA9685_SetPwmDutyCycle(PWMB, speed);
        if(dir == FORWARD) {
            PCA9685_SetLevel(BIN1, 0);
            PCA9685_SetLevel(BIN2, 1);
        } else {
            PCA9685_SetLevel(BIN1, 1);
            PCA9685_SetLevel(BIN2, 0);
        }
    }
}

// void  handler(int signo)
// {
// 	printf("\r\nHandler:Motor Stop\r\n");	
// 	stop_motor();
// 	DEV_ModuleExit();
// 	gpioTerminate();
// 	exit(0);
// }

int main(int argc, char *argv[])
{
	int i;
	
	// Setup
	if(DEV_ModuleInit())
		exit(0);
	
	if(gpioInitialise() < 0)
	{
		fprintf(stderr, "pigpio initialize failure\n");
		return 1;
	}
	
	gpioSetMode(BUTTON_PIN, PI_INPUT);
	signal(SIGINT, handler);
	
	// Wait for button press	
	while(!gpioRead(BUTTON_PIN)){}
	
	init_motor();
	
	// Speed forward 100 -> 15
	run_motor(0,100);
	sleep(2);
	for(i = 95; i >=15; i-=5)
	{
		run_motor(0,i);
		sleep(1);
	}
	
	stop_motor();
	sleep(1);
	
	// Speed backward 15 -> 100
	run_motor(1,15);
	for(i = 20; i <= 100; i+=5)
	{
		run_motor(1,i);
		sleep(1);
	}

	sleep(2);
	stop_motor(PWMB);	
	DEV_ModuleExit();
	gpioTerminate();
	return 0;
}
