#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <pthread.h>
#include "sensors.h"
#include <stdlib.h>
#include <signal.h>
#include "MotorDriver.h"

#define LEFT_MOTOR MOTORA
#define RIGHT_MOTOR MOTORB
#define BUTTON_PIN 26

void driving_logic(sensor *line_left, sensor *line_middle, sensor *line_right, sensor *start_stop_button);

#endif
