/*
 * Stepper.h
 *
 *  Created on: Oct 6, 2016
 *      Author: cosmo
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#include "LaserController.h"

class Stepper {
public:
	Stepper(uint8_t slavePin, uint8_t stepPin, uint8_t dirPin);
	void MoveAbsolute(int pos, int feedrate);
	void MoveRelative(int pos, int feedrate);
	inline int GetPosition() { return position; }
	inline float GetPositionMM() { return (float)position/(STEPS_PER_MM); }
	inline bool Moving() { return !stepper->distanceToGo() == 0; }
	void SetPosition(int pos);
	bool run();

	//Debugging
	inline AccelStepper* getStepper() { return stepper; }
private:
	AMIS30543 *driver;
	AccelStepper *stepper;

	int position;
};

#endif /* STEPPER_H_ */
