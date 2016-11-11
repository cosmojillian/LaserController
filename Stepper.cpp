/*
 * Stepper.cpp
 *
 *  Created on: Oct 6, 2016
 *      Author: cosmo
 */

#include "Stepper.h"

Stepper::Stepper(uint8_t slavePin, uint8_t stepPin, uint8_t dirPin) {
	driver = new AMIS30543();

	driver->init(slavePin);
	delay(1);

	driver->resetSettings();
	driver->setCurrentMilliamps(MOTOR_CURRENT);
	driver->setStepMode(MICROSTEPPING);
	driver->enableDriver();

	stepper = new AccelStepper(AccelStepper::DRIVER, stepPin, dirPin);

	stepper->setMaxSpeed(MAX_SPEED*STEPS_PER_MM);
	stepper->setAcceleration(MAX_ACCEL*STEPS_PER_MM);

	position = 0;
}

bool Stepper::run() {
	return stepper->run();
}

void Stepper::MoveAbsolute(int pos, int feedrate) {
	position = pos;

	if (this->position < 0) this->position = 0;
	else if (this->position > AXIS_MAX_LENGTH*STEPS_PER_MM) this->position = AXIS_MAX_LENGTH*STEPS_PER_MM;

	stepper->setMaxSpeed(feedrate);
	stepper->moveTo(this->position);
}

void Stepper::MoveRelative(int pos, int feedrate) {
	position += pos;

	if (this->position < 0) this->position = 0;
	else if (this->position > AXIS_MAX_LENGTH*STEPS_PER_MM) this->position = AXIS_MAX_LENGTH*STEPS_PER_MM;

	stepper->setMaxSpeed(feedrate);
	stepper->moveTo(this->position);
}

void Stepper::SetPosition(int pos) {
	position = pos;
	stepper->setCurrentPosition(position);
}

