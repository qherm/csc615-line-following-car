#include "main.h"

void driving_logic(sensor *left, sensor *middle, sensor *right, sensor *start_stop_button){
	while(!start_stop_button.read)
	{
    	printf("HERE\n");
		// LineSensor.read==1: sensor reads white
		// LineSensor.read==0: sensor reads black
		if(line_left.read && line_middle.read && line_right.read){
			// Maybe rotate in arbitrary direction
			Motor_Run(LEFT_MOTOR, FORWARD, 0);
			Motor_Run(RIGHT_MOTOR, FORWARD, 0);
		} else if(line_left.read && line_middle.read && !line_right.read){
			// Turn right
			Motor_Run(LEFT_MOTOR, FORWARD, 100);
			Motor_Run(RIGHT_MOTOR, FORWARD, 0);
		} else if(line_left.read && !line_middle.read && line_right.read){
			// Move forward
			Motor_Run(LEFT_MOTOR, FORWARD, 100);
			Motor_Run(RIGHT_MOTOR, FORWARD, 100);
		} else if(line_left.read && !line_middle.read && !line_right.read){
			// Rotate right
			Motor_Run(LEFT_MOTOR, FORWARD, 100);
			Motor_Run(RIGHT_MOTOR, BACKWARD, 100);
		} else if(!line_left.read && line_middle.read && line_right.read){
			// Turn left
			Motor_Run(LEFT_MOTOR, FORWARD, 0);
			Motor_Run(RIGHT_MOTOR, FORWARD, 100);
		} else if(!line_left.read && line_middle.read && !line_right.read){
			// Odd case. Probably stop until sensors read properly.
			Motor_Run(LEFT_MOTOR, FORWARD, 0);
			Motor_Run(RIGHT_MOTOR, FORWARD, 0);
		} else if(!line_left.read && !line_middle.read && line_right.read){
			// Rotate left.
			Motor_Run(LEFT_MOTOR, BACKWARD, 100);
			Motor_Run(RIGHT_MOTOR, FORWARD, 100);
		} else if(!line_left.read && !line_middle.read && !line_right.read){
			// Either wait for sensors to read properly or rotate in arbitrary direction.
			Motor_Run(LEFT_MOTOR, FORWARD, 0);
			Motor_Run(RIGHT_MOTOR, FORWARD, 0);
		}
	}
	return 0;
}

int main()
{
	if(gpioInitialise() < 0)
	{
		fprintf(stderr, "pigpio initialise failure\n");
		return 1;
	}

	gpioSetMode(IRL, PI_INPUT);
  gpioSetMode(IRM, PI_INPUT);
	gpioSetMode(IRR, PI_INPUT);
	gpioSetMode(BUTTON_PIN, PI_INPUT);

	pthread_t line_left_thread, line_middle_thread, line_right_thread,  start_stop_button_thread, object_middle_thread;
	int line_left_return, line_middle_return, line_right_return, start_stop_button_return, object_middle_return;
	
	sensor line_right, line_middle, line_left, object_middle, start_stop_button;

  line_left.pin = IRL;
  line_left.read = 0;

	line_middle.pin = IRM;
  line_middle.read = 0;

	line_right.pin = IRR;
	line_right.read = 0;

  start_stop_button.pin = BUTTON_PIN;
  start_stop_button.read = 0;

	line_left_return = pthread_create(&line_left_thread, NULL, sense, &line_left);
	line_middle_return = pthread_create(&line_middle_thread, NULL, sense, &line_middle);
  line_right_return = pthread_create(&line_right_thread, NULL, sense, &line_right);
  start_stop_button_return = pthread_create(&start_stop_button_thread, NULL, sense, &start_stop_button);

  while(!start_stop_button.read){}

	if(DEV_ModuleInit())
    	return 1;

	Motor_Init();

	printf("Driving in ");
	int i = 3;
	while(i>0){
		printf("%d\n",i);
		i--;
		sleep(1);
	}
	printf("GO!\n");

	Motor_Run(LEFT_MOTOR, FORWARD, 100);
	Motor_Run(RIGHT_MOTOR, FORWARD, 100);
	
	driving_logic(&line_left, &line_middle, &line_right, &start_stop_button);

  printf("I'm here");
	// pthread_join(obstacle_thread, NULL);
	pthread_join(line_left_thread, NULL);
	pthread_join(line_middle_thread, NULL);
	pthread_join(line_right_thread, NULL);
  pthread_join(start_stop_button_thread, NULL);
  	
	printf("stopping\n");
	Motor_Stop(MOTORA);
	Motor_Stop(MOTORB);
	gpioTerminate();
	DEV_ModuleExit();

	return 0;
}
