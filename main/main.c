#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <pthread.h>
#include "lib/sensors/sensors.h"

int main()
{
	if(gpioInitialise() < 0)
	{
		fprintf(stderr, "pigpio initialise failure\n");
		return 1;
	}
	
	gpioSetMode(TRL, PI_INPUT);
    gpioSetMode(TRM, PI_INPUT)
	gpioSetMode(TRR, PI_INPUT);

	pthread_t line_left, line_middle, line_right, object_middle;
	int obstacle_return, line_return;
	
	sensor obstacle, line;

	obstacle.pin = PINOA;
	obstacle.read = 0;

	line.pin = PINLS;
	line.read = 0;

	obstacle_return = pthread_create(&obstacle_thread, NULL, sense, &obstacle);
	line_return = pthread_create(&line_thread, NULL, sense, &line);
	
	while(1)
	{
		printf("Line read: %i\n", line.read);
		printf("Obstacle read: %i\n\n", obstacle.read);
		usleep(500000);
	}

	pthread_join(obstacle_thread, NULL);
	pthread_join(line_thread, NULL);

	printf("Obstacle thread returns: %i\n", obstacle_return);
	printf("Line thread returns: %i\n", line_return);

	return 0;
}
