/*
 * Platform.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: Caroline
 */

#include "Platform.h"

Platform::Platform() {
	SPI.begin();
	currentFeedrate = MAX_SPEED;
	x = new Stepper(slavePinX, X_STEP_PIN, X_DIR_PIN);
	y = new Stepper(slavePinY, Y_STEP_PIN, Y_DIR_PIN);

	pinMode(X_MIN_PIN, INPUT_PULLUP);
	pinMode(X_MAX_PIN, INPUT_PULLUP);
	pinMode(Y_MIN_PIN, INPUT_PULLUP);
	pinMode(Y_MAX_PIN, INPUT_PULLUP);

	relative = false;
	state = Ready;
}

void Platform::Update() {
	x->run();
	y->run();

	if (debugTimer >= 2000) {
		debugTimer -= 2000;
	}

	if (!x->Moving() && !y->Moving()) {
		if (state != Ready) {
			state = Ready;
		}
	}

	if (state == Homing) {
		if (CheckEndstop(0, true)) {
			SetPositionX(0);
		}

		if (CheckEndstop(1, true)) {
			SetPositionY(0);
		}
	} else if (state == Moving) {
		if (CheckEndstop(0, true)) {
			SetPositionX(0);
			//TODO Should we be at home? if not then do an error thing
		}

		if (CheckEndstop(1, true)) {
			SetPositionY(0);
			//TODO Should we be at home? if not then do an error thing
		}

		if (CheckEndstop(0, false)) {
			//TODO Should we be at max? if not then do an error thing?
		}

		if (CheckEndstop(1, false)) {
			//TODO Should we be at max? if not then do an error thing?
		}
	}
}

void Platform::ReportPosition() {
	Serial.println("Current Position:");
	Serial.print("X: ");
	Serial.print(x->GetPositionMM(), 3);
	Serial.print("	Y: ");
	Serial.println(y->GetPositionMM(), 3);
}

void Platform::ReportEndstops() {
	Serial.println("Endstop status:");
	Serial.print("X MIN: ");
	Serial.print(CheckEndstop(0, true));
	Serial.print("	X MAX: ");
	Serial.print(CheckEndstop(0, false));
	Serial.print("	Y MIN: ");
	Serial.print(CheckEndstop(1, true));
	Serial.print("	X MAX: ");
	Serial.println(CheckEndstop(1, false));
}

int Platform::CheckEndstop(int axis, bool min) {
	if (axis == 0) { //X axis
		if (min) {
			return digitalReadFast(X_MIN_PIN);
		} else {
			return digitalReadFast(X_MAX_PIN);
		}
	} else { //Y axis
		if (min) {
			return digitalReadFast(Y_MIN_PIN);
		} else {
			return digitalReadFast(Y_MAX_PIN);
		}
	}
}

void Platform::MoveLinear(float _x, float _y, float feedrate) {
	state = Moving;

	if (feedrate > MAX_SPEED)
		feedrate = MAX_SPEED;
	else if (feedrate <= 0)
		feedrate = currentFeedrate;

	float xFeed = feedrate, yFeed = feedrate;
	int xPos = (int) _x * STEPS_PER_MM, yPos = (int) _y * STEPS_PER_MM;

	if (relative) {
		float xPosition = _x, yPosition = _y;
		float totalDistance = sqrt(_x * _x + _y * _y);
		float moveTime = totalDistance / feedrate;

		xFeed = abs(xPosition) / moveTime;
		yFeed = abs(yPosition) / moveTime;

		x->MoveRelative(xPos, xFeed * STEPS_PER_MM);
		y->MoveRelative(yPos, yFeed * STEPS_PER_MM);
	} else {
		float xDist = _x - x->GetPositionMM(), yDist = _y - y->GetPositionMM();
		float totalDistance = sqrt(xDist * xDist + yDist * yDist); //x or y - current x or y = x or y distance
		float moveTime = totalDistance / feedrate;

		xFeed = abs(xDist) / moveTime;
		yFeed = abs(yDist) / moveTime;

//		Serial.print("Total distance is ");
//		Serial.print(totalDistance);
//		Serial.print(", we are moving at ");
//		Serial.print(feedrate);
//		Serial.print(", We have ");
//		Serial.print(moveTime);
//		Serial.print(" seconds to move, and we need to move at ");
//		Serial.print(xFeed);
//		Serial.print(",");
//		Serial.println(yFeed);
//		Serial.print("X will arrive in ");
//		Serial.print(abs(xDist)/xFeed);
//		Serial.print(" seconds, and Y will arrive in ");
//		Serial.println(abs(yDist)/yFeed);

		x->MoveAbsolute(xPos, xFeed * STEPS_PER_MM);
		y->MoveAbsolute(yPos, yFeed * STEPS_PER_MM);
	}

	currentFeedrate = feedrate;
}

void Platform::MoveLinear(float position, float feedrate, bool _y) {
	state = Moving;

	if (feedrate > MAX_SPEED)
		feedrate = MAX_SPEED;
	else if (feedrate <= 0)
		feedrate = currentFeedrate;

	if (relative) {
		if (_y) {
			y->MoveRelative(position * STEPS_PER_MM, feedrate * STEPS_PER_MM);
		} else {
			x->MoveRelative(position * STEPS_PER_MM, feedrate * STEPS_PER_MM);
		}
	} else {
		if (_y) {
			y->MoveAbsolute(position * STEPS_PER_MM, feedrate * STEPS_PER_MM);
		} else {
			x->MoveAbsolute(position * STEPS_PER_MM, feedrate * STEPS_PER_MM);
		}
	}

	currentFeedrate = feedrate;
}

void Platform::SetAbsolute() {
	relative = false;
}

void Platform::SetRelative() {
	relative = true;
}

void Platform::SetPositionX(float position) {
	x->SetPosition(position * STEPS_PER_MM);
}

void Platform::SetPositionY(float position) {
	y->SetPosition(position * STEPS_PER_MM);
}

void Platform::HomeAllAxes() {
	state = Homing;

	SetPositionX(AXIS_MAX_LENGTH * 2);
	SetPositionY(AXIS_MAX_LENGTH * 2);

	x->MoveAbsolute(0, HOMING_FEEDRATE * STEPS_PER_MM);
	y->MoveAbsolute(0, HOMING_FEEDRATE * STEPS_PER_MM);
}

