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

#define MICROSTEPPING 4
#define MOTOR_STEPS_PER_REV 200
#define DRIVE_PITCH 2
#define STEPS_PER_MM MICROSTEPPING*MOTOR_STEPS_PER_REV*DRIVE_PITCH
#define MAX_SPEED 5 //mm per sec
#define MAX_ACCEL 50 //mm per sec^2

//Machine limits
#define AXIS_MAX_LENGTH 50
#define HOMING_FEEDRATE 2

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
#define MOTOR_CURRENT 500

#endif /* CONFIGURATION_H_ */
