#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <pthread.h>
#include "sensors.h"

int main()
{
	if(gpioInitialise() < 0)
	{
		fprintf(stderr, "pigpio initialise failure\n");
		return 1;
	}
	
	gpioSetMode(TRL, PI_INPUT);
    gpioSetMode(TRM, PI_INPUT);
	gpioSetMode(TRR, PI_INPUT);

	pthread_t line_left_thread, line_middle_thread, line_right_thread, object_middle_thread;
	int line_left_return, line_middle_return, line_right_return, object_middle_return;
	
	sensor line_right, line_middle, line_left, object_middle;

    line_left.pin = TRL;
    line_left.read = 0;

    line_middle.pin = TRM;
    line_middle.read = 0;

	line_right.pin = TRR;
	line_right.read = 0;

	line_left_return = pthread_create(&line_left_thread, NULL, sense, &line_left);
	line_middle_return = pthread_create(&line_middle_thread, NULL, sense, &line_middle);
    line_right_return = pthread_create(&line_right_thread, NULL, sense, &line_right);
	
	while(1)
	{
		printf("Line left read: %i\n", line_left.read);
		printf("Line middle read: %i\n", line_middle.read);
		printf("Line right read: %i\n", line_right.read);
		usleep(500000);
	}

	// pthread_join(obstacle_thread, NULL);
	pthread_join(line_left_thread, NULL);
	pthread_join(line_middle_thread, NULL);
	pthread_join(line_right_thread, NULL);

	// printf("Obstacle thread returns: %i\n", obstacle_return);
	// printf("Line thread returns: %i\n", line_return);

	return 0;
}
