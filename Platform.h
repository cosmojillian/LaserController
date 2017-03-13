/*
 * Platform.h
 *
 *  Created on: Sep 29, 2016
 *      Author: Caroline
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "LaserController.h"

const uint8_t slavePinY = 10;
const uint8_t slavePinX = 9;

enum MoveState {
	Ready,
	Homing,
	Moving
};

class Platform {
public:
	Platform();
	void Update();
	void MoveLinear(float _x, float _y, float feedrate);
	void MoveLinear(float position, float feedrate, bool _y);
	void MoveRapid(float _x, float _y, float feedrate);
	void SetAbsolute();
	void SetRelative();
	void SetPositionX(float position);
	void SetPositionY(float position);
	void HomeAllAxes();
	void ReportEndstops();
	void ReportPosition();
	inline MoveState GetState() { return state; }
	int CheckEndstop(int axis, bool min);
private:
	MoveState state;
	Stepper *x;
	Stepper *y;

	elapsedMillis debugTimer;

	bool relative;
	float currentFeedrate;
};

#endif /* PLATFORM_H_ */
