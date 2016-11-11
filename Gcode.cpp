/*
 * Gcode.cpp
 *
 *  Created on: Sep 28, 2016
 *      Author: cosmo
 */

#include "Gcode.h"

/**
 * G0-Rapid Linear movement (X- Y- F-feedrate)
 * G1-Linear Movement (X- Y- F-feedrate)
 * G2-Clockwise Arc
 * G3-Anticlockwise Arc
 * G4-Dwell (P-time in ms S-time in seconds)
 * G20-Set units to inches
 * G21-Set units to mm
 * G28-Home axes (X,Y-flag independent axes, all if empty)
 * G90-Absolute positioning
 * G91-Relative positioning
 * G92-Set position (X-new X position Y-new Y position)
 * M0-Stop (P-wait time in ms S-wait time in seconds
 * M1-Sleep
 * M17-Power motors (X Y)
 * M18-Disable motors (X Y)
 * M110-Set Current Line Number
 * M111-Enable debug (S-on/off)
 * M112-Emergency stop
 * M114-Get current position
 * M115-Get version
 * M119-Get endstop status
 * M206-Offset axes (X Y Z)
 */

Gcode::Gcode(Platform *_platform) {
	platform = _platform;
	serialBuffer = new CodeBuffer();
	state = Idle;

	waitTime = 0;
}

void Gcode::Update() {
	serialBuffer->FillCodeBuffer();

	if (state == Idle) {
		if (!serialBuffer->IsEmpty()) {
			if (serialBuffer->Seen('G')) {
				ExecuteGCode();
			} else if (serialBuffer->Seen('M')) {
				ExecuteMCode();
			} else {
				Serial.print("Unrecognized ");
				Serial.println(serialBuffer->CurrentCode());
				//unrecognized code (Spit out code)
			}
			serialBuffer->CommandExecuted();
			state = Executing;
		}
	} else {
		if (waitTime > 0) {
			if (waitTimer >= waitTime) {
				waitTime = 0;
				Serial.println("ok");
				state = Idle;
			}
		} else {
			if (platform->GetState() == Ready) {
				Serial.println("ok");
				state = Idle;
			}
		}
	}
}

void Gcode::ExecuteGCode() {
	float x = 0, y = 0, f = 0;
	int s = 0, p = 0;
	bool _x = false, _y = false;

	switch (serialBuffer->GetIntValue()) {
	case 0:		//G0-Rapid Linear movement (X- Y- F-feedrate)
//		Serial.println("Received G0");
//		if (serialBuffer->Seen('X')) {
//
//		}
//		if (serialBuffer->Seen('Y')) {
//
//		}
//		if (serialBuffer->Seen('F')) {
//
//		}
//		break;
	case 1:		//G1-Linear Movement (X- Y- F-feedrate)
		if (serialBuffer->Seen('X')) {
			x = serialBuffer->GetFloatValue();
			_x = true;
		}
		if (serialBuffer->Seen('Y')) {
			y = serialBuffer->GetFloatValue();
			_y = true;
		}
		if (serialBuffer->Seen('F')) {
			f = serialBuffer->GetFloatValue();
		}

		if (_x) {
			if (_y)
				platform->MoveLinear(x, y, f);
			else
				platform->MoveLinear(x, f, false);
		} else if (_y)
			platform->MoveLinear(y, f, true);
		break;
	case 2:		//G2-Clockwise Arc
		Serial.println("G2 not yet supported");
		break;
	case 3:		//G3-Anticlockwise Arc
		Serial.println("G3 not yet supported");
		break;
	case 4: 	//G4-Dwell (P-time in ms S-time in seconds)
		if (serialBuffer->Seen('P')) {
			p = serialBuffer->GetIntValue();
		}
		if (serialBuffer->Seen('S')) {
			s = serialBuffer->GetIntValue();
		}

		waitTime = p + s * 1000;
		waitTimer = 0;
		break;
	case 20:	//G20-Set units to inches
		Serial.println("G20 not yet supported");
		break;
	case 21:	//G21-Set units to mm
		Serial.println("G21 not yet supported");
		break;
	case 28: 	//G28-Home axes (X,Y-flag independent axes, all if empty)
		if (serialBuffer->Seen('X')) {

		} else if (serialBuffer->Seen('Y')) {

		} else {
			platform->HomeAllAxes();
		}
		break;
	case 90: 	//G90-Absolute positioning
		platform->SetAbsolute();
		break;
	case 91: 	//G91-Relative positioning
		platform->SetRelative();
		break;
	case 92: 	//G92-Set position (X-new X position Y-new Y position)
		if (serialBuffer->Seen('X')) {
			platform->SetPositionX(serialBuffer->GetFloatValue());
		}
		if (serialBuffer->Seen('Y')) {
			platform->SetPositionX(serialBuffer->GetFloatValue());
		}
		break;
	default:
		Serial.println("Unrecognized G codes");
		//Report unrecognized command
		break;
	}
}

void Gcode::ExecuteMCode() {
	int s = 0, p = 0;

	switch (serialBuffer->GetIntValue()) {
	case 0: 	//M0-Stop (P-wait time in ms S-wait time in seconds
		break;
		//M1-Sleep
		//M17-Power motors (X Y)
		//M18-Disable motors (X Y)
		//M110-Set Current Line Number
		//M111-Enable debug (S-on/off)
		//M112-Emergency stop
	case 114: 	//M114-Get current position
		platform->ReportPosition();
		break;
	case 115: 	//M115-Get version
		Serial.print("Version ");
		Serial.println(VERSION);
		break;
	case 119: 	//M119-Get endstop status
		platform->ReportEndstops();
		break;
		//M206-Offset axes (X Y)
	default:
		Serial.println("Unrecognized M code");
		break;
	}
}

