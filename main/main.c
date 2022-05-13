#include "main.h"

#define FW 1
#define BW 0

// Counts from 3 to 1
void countdown(){
	printf("Driving in ");
	int i = 3;
	while(i>0){
		printf("%d\n",i);
		i--;
		sleep(1);
	}

	printf("GO!\n");
}

// Stops motors, terminates pigpio, and clears out
// any memory allocated for the DEV module
void stop_all(){
	printf("Stopping\n");
	Motor_Stop(MOTORA);
	Motor_Stop(MOTORB);
	gpioTerminate();
	DEV_ModuleExit();
}

void driving_logic(sensor *line_left, sensor *line_middle, sensor *line_right, sensor *start_stop_button, sensor *obstacle_middle){
	while(!start_stop_button->read){
		// LineSensor.read==1:      sensor reads white
		// LineSensor.read==0:      sensor reads black
    // ObstacleSensor.read==1:  sensor sees no object
    // ObjectSensor.read==0:    sensor sees an object
		if(!obstacle_middle->read){
			Motor_Stop(LEFT_MOTOR);
			Motor_Stop(RIGHT_MOTOR);
			sleep(5);
			if(!obstacle_middle->read){
				// If obstacle still there after ~5 seconds, start obstacle avoidance
				avoid_obstacle(line_left, line_middle, line_right, start_stop_button, obstacle_middle);
			}
		} else if(!line_left->read && !line_middle->read && !line_right->read){
			// Maybe rotate in arbitrary direction
			Motor_Run(LEFT_MOTOR, FW, 0);
			Motor_Run(RIGHT_MOTOR, FW, 0);
		} else if(!line_left->read && !line_middle->read && line_right->read){
			// Turn right
			Motor_Run(LEFT_MOTOR, FW, 100);
			Motor_Run(RIGHT_MOTOR, FW, 0);
		} else if(!line_left->read && line_middle->read && !line_right->read){
			// Move forward
			Motor_Run(LEFT_MOTOR, FW, 100);
			Motor_Run(RIGHT_MOTOR, FW, 100);
		} else if(!line_left->read && line_middle->read && line_right->read){
			// Rotate right
			Motor_Run(LEFT_MOTOR, FW, 100);
			Motor_Run(RIGHT_MOTOR, BACKWARD, 100);
		} else if(line_left->read && !line_middle->read && !line_right->read){
			// Turn left
			Motor_Run(LEFT_MOTOR, FW, 0);
			Motor_Run(RIGHT_MOTOR, FW, 100);
		} else if(line_left->read && !line_middle->read && line_right->read){
			// Odd case. Probably stop until sensors read properly.
			Motor_Run(LEFT_MOTOR, FW, 0);
			Motor_Run(RIGHT_MOTOR, FW, 0);
		} else if(line_left->read && line_middle->read && !line_right->read){
			// Rotate left.
			Motor_Run(LEFT_MOTOR, BACKWARD, 100);
			Motor_Run(RIGHT_MOTOR, FW, 100);
		} else if(line_left->read && line_middle->read && line_right->read){
			// Either wait for sensors to read properly or rotate in arbitrary direction.
			Motor_Run(LEFT_MOTOR, FW, 0);
			Motor_Run(RIGHT_MOTOR, FW, 0);
		}
	}
	printf("Driving finished\n"); // 2. Prints properly
	return;
}

void avoid_obstacle(sensor *line_left, sensor *line_middle, sensor *line_right, sensor *start_stop_button, sensor *obstacle_middle){
	// Rotate left 90 degrees
	Motor_Run(LEFT_MOTOR, BACKWARD, 100);
	Motor_Run(RIGHT_MOTOR, FW, 100);
	sleep(1);

	// Move forward for a moment
	Motor_Run(LEFT_MOTOR, FW, 100);
	Motor_Run(RIGHT_MOTOR, FW, 100);
	sleep(2);

	// Rotate right 90 degrees
	Motor_Run(LEFT_MOTOR, FW, 100);
	Motor_Run(RIGHT_MOTOR, BACKWARD, 100);
	sleep(1);

	// Move forward until see line
	Motor_Run(LEFT_MOTOR, FW, 100);
	Motor_Run(RIGHT_MOTOR, FW, 100);
	sleep(2);
	while(line_left->read && line_middle->read && line_right->read){}

	return;
}

int main()
{ 
  printf("f:%d\n", FW);
  printf("b:%d\n", BACKWARD);
	if(gpioInitialise() < 0)
	{
		fprintf(stderr, "pigpio initialise failure\n");
		return 1;
	}

	gpioSetMode(IRL, PI_INPUT);
	gpioSetMode(IRM, PI_INPUT);
	gpioSetMode(IRR, PI_INPUT);
	gpioSetMode(BUTTON_PIN, PI_INPUT);
	gpioSetMode(TRM, PI_INPUT);

	pthread_t line_left_thread, line_middle_thread, line_right_thread,  start_stop_button_thread, obstacle_middle_thread;
	sensor line_left, line_middle, line_right, start_stop_button, obstacle_middle;

	line_left.pin = IRL;
	line_left.read = 0;
	line_left.cont = true;

	line_middle.pin = IRM;
	line_middle.read = 0;
	line_middle.cont = true;

	line_right.pin = IRR;
	line_right.read = 0;
	line_right.cont = true;

	start_stop_button.pin = BUTTON_PIN;
	start_stop_button.read = 0;
	start_stop_button.cont = true;

	obstacle_middle.pin = TRM;
	obstacle_middle.read = 0;
	obstacle_middle.cont = true;

	pthread_create(&line_left_thread, NULL, sense, &line_left);
	pthread_create(&line_middle_thread, NULL, sense, &line_middle);
	pthread_create(&line_right_thread, NULL, sense, &line_right);
	pthread_create(&start_stop_button_thread, NULL, sense, &start_stop_button);
	pthread_create(&obstacle_middle_thread, NULL, sense, &obstacle_middle);

	while(1){
		printf("LEFT: %d", line_left.read);
		printf("MIDDLE: %d", line_middle.read);
		printf("RIGHT: %d", line_right.read);
	}
	return 0;
	while(!start_stop_button.read){}

	if(DEV_ModuleInit())
    	return 1;

	Motor_Init();

	countdown();
	
	driving_logic(&line_left, &line_middle, &line_right, &start_stop_button, &obstacle_middle);

	line_left.cont = false;
	line_middle.cont = false;
	line_right.cont = false;
	obstacle_middle.cont = false;
	start_stop_button.cont = false;
	
	pthread_join(obstacle_middle_thread, NULL);
	pthread_join(line_left_thread, NULL);
	pthread_join(line_middle_thread, NULL);
	pthread_join(line_right_thread, NULL);
  pthread_join(start_stop_button_thread, NULL);
  	
	stop_all();

	return 0;
}
