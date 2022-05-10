#include "main.h"

void Handler(int signo)
{
  printf("stopping\n");
  Motor_Stop(MOTORA);
  Motor_Stop(MOTORB);
  gpioTerminate();
  DEV_ModuleExit();

  exit(0);
}

int main()
{
  	signal(SIGINT, Handler);
	if(gpioInitialise() < 0)
	{
		fprintf(stderr, "pigpio initialise failure\n");
		return 1;
	}
	
	gpioSetMode(IRL, PI_INPUT);
  	gpioSetMode(IRM, PI_INPUT);
	gpioSetMode(IRR, PI_INPUT);

  	printf("left pin %d",IRL);
  	printf("right pin %d",IRR);
  	printf("middle pin %d",IRM);

	// pthread_t line_left_thread, line_middle_thread, line_right_thread, object_middle_thread;
	// int line_left_return, line_middle_return, line_right_return, object_middle_return;
	
	// sensor line_right, line_middle, line_left, object_middle;

  	// line_left.pin = IRL;
  	// line_left.read = 0;

	// line_middle.pin = IRM;
  	// line_middle.read = 0;

	// line_right.pin = IRR;
	// line_right.read = 0;

	// line_left_return = pthread_create(&line_left_thread, NULL, sense, &line_left);
	// line_middle_return = pthread_create(&line_middle_thread, NULL, sense, &line_middle);
  	// line_right_return = pthread_create(&line_right_thread, NULL, sense, &line_right);

	if(DEV_ModuleInit())
    	return 1;

	Motor_Init();

	Motor_Run(MOTORA, FORWARD, 100);
	Motor_Run(MOTORB, FORWARD, 100);
	
	while(1)
	{
		// LineSensor.read==1: sensor reads white
		// LineSensor.read==0: sensor reads black
		if(line_left.read && line_middle.read && line_right.read){
			// Maybe rotate in arbitrary direction
		} else if(line_left.read && line_middle.read && !line_right.read){
			// Turn right
		} else if(line_left.read && !line_middle.read && line_right.read){
			// Move forward
			Motor_Run(MOTORA, FORWARD, 100);
			Motor_Run(MOTORB, FORWARD, 100);
		} else if(line_left.read && !line_middle.read && !line_right.read){
			// Rotate right
		} else if(!line_left.read && line_middle.read && line_right.read){
			// Turn left
		} else if(!line_left.read && line_middle.read && !line_right.read){
			// Odd case. Probably stop until sensors read properly.
		} else if(!line_left.read && !line_middle.read && line_right.read){
			// Rotate left.
		} else if(!line_left.read && !line_middle.read && !line_right.read){
			// Either wait for sensors to read properly or rotate in arbitrary direction.
		}
		// printf("Line left read: %i\n", line_left.read);
		// printf("Line middle read: %i\n", line_middle.read);
		// printf("Line right read: %i\n", line_right.read);
		usleep(500000);
	}

	// pthread_join(obstacle_thread, NULL);
	// pthread_join(line_left_thread, NULL);
	// pthread_join(line_middle_thread, NULL);
	// pthread_join(line_right_thread, NULL);
  	
	DEV_ModuleExit();

	return 0;
}
