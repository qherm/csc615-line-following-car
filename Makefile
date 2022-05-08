# Name: Shane Waxler
# Student ID: 918415347
# Assignment 3 - Start Your Motors
# Date: 03/18/2022

DIR_OBJ = ./lib
DIR_BIN = ./bin
DIR_Config = ./lib/Config
DIR_MotorDriver = ./lib/MotorDriver
DIR_PCA9685 = ./lib/PCA9685

DIR_run_motor = ./lib/run_motor
DIR_sensors = ./lib/sensors
DIR_ls7336r = ./lib/ls7336r

DIR_Examples = ./main

OBJ_C = $(wildcard ${DIR_OBJ}/*.c ${DIR_Examples}/*.c ${DIR_Config}/*.c ${DIR_MotorDriver}/*.c ${DIR_PCA9685}/*.c )
OBJ_O = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${OBJ_C}))

TARGET = start_car
#BIN_TARGET = ${DIR_BIN}/${TARGET}

CC = gcc

DEBUG = -g -O0 -Wall
CFLAGS += $(DEBUG)

# USELIB = USE_BCM2835_LIB
# USELIB = USE_WIRINGPI_LIB
USELIB = USE_DEV_LIB
DEBUG = -D $(USELIB) 
ifeq ($(USELIB), USE_BCM2835_LIB)
    LIB = -lbcm2835 -lm 
else ifeq ($(USELIB), USE_WIRINGPI_LIB)
    LIB = -lwiringPi -lm 

endif

LIB = -lpigpio -lrt -lwiringPi -lm 

# DIR_run_motor = ./lib/run_motor
# DIR_sensors = ./lib/sensors
# DIR_ls7336r = ./lib/ls7336r

${TARGET}:${OBJ_O}
	$(CC) $(CFLAGS) $(OBJ_O) -o $@ $(LIB) -lm

${DIR_BIN}/%.o : $(DIR_Examples)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I $(DIR_OBJ) -I $(DIR_Config) -I $(DIR_MotorDriver) -I $(DIR_PCA9685)

${DIR_BIN}/%.o : $(DIR_OBJ)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I $(DIR_Config)
    
${DIR_BIN}/%.o : $(DIR_Config)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB)

${DIR_BIN}/%.o : $(DIR_run_motor)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I $(DIR_Config)

${DIR_BIN}/%.o : $(DIR_sensors)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I $(DIR_Config)
	
${DIR_BIN}/%.o : $(DIR_ls7336r)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I $(DIR_Config)

${DIR_BIN}/%.o : $(DIR_PCA9685)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I $(DIR_Config)

${DIR_BIN}/%.o : $(DIR_MotorDriver)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I $(DIR_Config) -I $(DIR_PCA9685)


clean :
	rm $(DIR_BIN)/*.* 
	rm $(TARGET) 
