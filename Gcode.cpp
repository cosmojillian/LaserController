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
}

void Gcode::Update() {
	serialBuffer->FillCodeBuffer();

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
	}
}

void Gcode::ExecuteGCode() {
	float x = 0, y = 0, f = 0;

	switch (serialBuffer->GetIntValue()) {
	case 0:		//G0-Rapid Linear movement (X- Y- F-feedrate)
		Serial.println("Received G0");
		if (serialBuffer->Seen('X')) {

		}
		if (serialBuffer->Seen('Y')) {

		}
		if (serialBuffer->Seen('F')) {

		}
		break;
	case 1:		//G1-Linear Movement (X- Y- F-feedrate)
		if (serialBuffer->Seen('X')) {
			x = serialBuffer->GetFloatValue();
		}
		if (serialBuffer->Seen('Y')) {
			y = serialBuffer->GetFloatValue();
		}
		if (serialBuffer->Seen('F')) {
			f = serialBuffer->GetFloatValue();
		}
		platform->MoveLinear(x, y, f);
		break;
	case 2:		//G2-Clockwise Arc
		Serial.println("G2 not yet supported");
		break;
	case 3:		//G3-Anticlockwise Arc
		Serial.println("G3 not yet supported");
		break;
	case 4: 	//G4-Dwell (P-time in ms S-time in seconds)
		Serial.println("Received G4");
		break;
	case 20:	//G20-Set units to inches
		Serial.println("Received G20");
		break;
	case 21:	//G21-Set units to mm
		Serial.println("Received G21");
		break;
	default:
		Serial.println("Unrecognized G command");
		//Report unrecognized command
		break;
	}
}

void Gcode::ExecuteMCode() {
	switch (serialBuffer->GetIntValue()) {
	case 119:
		platform->ReportEndstops();
		break;
	}
}

