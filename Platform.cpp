/*
 * Platform.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: cosmo
 */

#include "Platform.h"

Platform::Platform() {
	currentFeedrate = MAX_SPEED;
	x = new AMIS30543();
	y = new AMIS30543();

	pinMode(X_MIN_PIN, INPUT_PULLUP);
	pinMode(X_MAX_PIN, INPUT_PULLUP);
	pinMode(Y_MIN_PIN, INPUT_PULLUP);
	pinMode(Y_MAX_PIN, INPUT_PULLUP);

	SPI.begin();
	x->init(slavePinX);
	y->init(slavePinY);
	delay(1);

	x->resetSettings();
	x->setCurrentMilliamps(500);
	x->setStepMode(16);
	x->enableDriver();

	y->resetSettings();
	y->setCurrentMilliamps(500);
	y->setStepMode(16);
	y->enableDriver();

	xStep = new AccelStepper(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
	yStep = new AccelStepper(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);

	xStep->setMaxSpeed(MAX_SPEED*STEPS_PER_MM);
	xStep->setAcceleration(MAX_ACCEL*STEPS_PER_MM);
	yStep->setMaxSpeed(MAX_SPEED*STEPS_PER_MM);
	yStep->setAcceleration(MAX_ACCEL*STEPS_PER_MM);
}

void Platform::Update() {
	xStep->run();
	yStep->run();
}

void Platform::ReportEndstops() {
	Serial.println("Endstop status:");
	Serial.print("X MIN: ");
	Serial.print(CheckEndstop(0, true));
	Serial.print("  X MAX: ");
	Serial.print(CheckEndstop(0, false));
	Serial.print("  Y MIN: ");
	Serial.print(CheckEndstop(1, true));
	Serial.print("  X MAX: ");
	Serial.println(CheckEndstop(1, false));
}

int Platform::CheckEndstop(int axis, bool min) {
	if (axis == 0) { //X axis
		if (min) {
			return digitalRead(X_MIN_PIN);
		} else {
			return digitalRead(X_MAX_PIN);
		}
	} else { //Y axis
		if (min) {
			return digitalRead(Y_MIN_PIN);
		} else {
			return digitalRead(Y_MAX_PIN);
		}
	}
}

void Platform::MoveLinear(float _x, float _y, float feedrate) {
	if (feedrate > MAX_SPEED) feedrate = MAX_SPEED;
	else if (feedrate <= 0) feedrate = currentFeedrate;

	float xFeed = feedrate, yFeed = feedrate;
	int xPos = (int) _x * STEPS_PER_MM, yPos = (int) _y * STEPS_PER_MM;

	if (xPos != 0) {
		if (yPos != 0) {
			float totalDistance = sqrt(_x*_x+_y*_y);
			float moveTime = totalDistance*feedrate;

			xFeed = xPos/moveTime;
			yFeed = yPos/moveTime;

			xStep->moveTo(xPos);
			xStep->setMaxSpeed(abs(xFeed*STEPS_PER_MM));
			yStep->moveTo(yPos);
			yStep->setMaxSpeed(abs(yFeed*STEPS_PER_MM));

			Serial.println("Diagonal move");
		} else {
			Serial.println("X move");
			xStep->moveTo(xPos);
			xStep->setMaxSpeed(xFeed*STEPS_PER_MM);
		}
	} else if (yPos != 0) {
		Serial.println("Y move");
		yStep->moveTo(yPos);
		yStep->setMaxSpeed(yFeed*STEPS_PER_MM);
	}

	currentFeedrate = feedrate;
}

