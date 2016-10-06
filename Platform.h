/*
 * Platform.h
 *
 *  Created on: Sep 29, 2016
 *      Author: cosmo
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "LaserController.h"

const uint8_t slavePinY = 10;
const uint8_t slavePinX = 9;

class Platform {
public:
	Platform();
	void Update();
	void MoveLinear(float _x, float _y, float feedrate);
	void MoveRapid(float _x, float _y, float feedrate);
	void ReportEndstops();
	int CheckEndstop(int axis, bool min);
private:
	AMIS30543 *x;
	AMIS30543 *y;
	AccelStepper *xStep;
	AccelStepper *yStep;

	float currentFeedrate;
};

#endif /* PLATFORM_H_ */
