/*
 * Configuration.h
 *
 *  Created on: Sep 28, 2016
 *      Author: cosmo
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#define VERSION "1.0"

//Serial settings
#define BAUD_RATE 115200
#define BUFFER_LENGTH 16
#define MAX_COMMAND_LENGTH 100

#define STEPS_PER_MM 6400 //16*200*2
#define MAX_SPEED 5 //mm per sec
#define MAX_ACCEL 50 //mm per sec^2

//Endstop settings
#define X_MAX_PIN 14
#define X_MIN_PIN 16
#define Y_MAX_PIN 17
#define Y_MIN_PIN 19

//Stepper drive settings
#define X_STEP_PIN 3
#define X_DIR_PIN 2
#define Y_STEP_PIN 5
#define Y_DIR_PIN 4

#endif /* CONFIGURATION_H_ */
