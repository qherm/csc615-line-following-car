#include "SpeedTurningControls.h"
#include <pigpio.h>
#include <pthread.h>
#include <stdio.h>

bool readOut = true;
void* speedReadOut(void*){
  double speedReadLeft = 0, speedReadRight = 0;
  while(readOut){
    speedReadLeft = getInstantCMPS(LEFT);
    speedReadRight = getInstantCMPS(RIGHT);
    println("Left wheel: " + speedReadLeft + " cm/s");
    println("Right wheel: " + speedReadRight + " cm/s");
  }
}

int main(){
  if(gpioInitialise() < 0){
    return -1;
  }
  if(initControls() < 0){
    return -1;
  }

  pthread_t instantSpeedCalc, readSpeed, balanceSpeed;

  pthread_create(&intantSpeedCalc, NULL, calculateInstantSpeed, NULL);
  pthread_create(&readSpeed, NULL, speedReadOut, NULL);

  setSpeed(100);
  sleep(2);
  reverse();
  sleep(2);
  stop();

  readOut = false;
  pthread_join(readSpeed, NULL);
  pthread_join(instantSpeedCalc, NULL);

  gpioTerminate();
}