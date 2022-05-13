#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <pthread.h>
#include "sensors.h"
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include "MotorDriver.h"

#define LEFT_MOTOR MOTORA
#define RIGHT_MOTOR MOTORB

void countdown();
void stop_all();
void driving_logic(sensor *line_left, sensor *line_middle, sensor *line_right, sensor *start_stop_button, sensor* obstacle_middle);
void avoid_obstacle(sensor *line_left, sensor *line_middle, sensor *line_right, sensor *start_stop_button, sensor *obstacle_middle);

#endif
