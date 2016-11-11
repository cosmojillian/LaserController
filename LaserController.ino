#include "LaserController.h"

/**
 * List of Gcode commands (# means it has been implemented):
 *
 * G0-Rapid Linear movement (X- Y- F-feedrate)
 * # G1-Linear Movement (X- Y- F-feedrate)
 * G2-Clockwise Arc
 * G3-Anticlockwise Arc
 * # G4-Dwell (P-time in ms S-time in seconds)
 * G20-Set units to inches
 * G21-Set units to mm
 * # G28-Home axes (X,Y-flag independent axes, all if empty)
 * # G90-Absolute positioning
 * # G91-Relative positioning
 * # G92-Set position (X-new X position Y-new Y position)
 * M0-Stop (P-wait time in ms S-wait time in seconds
 * M1-Sleep
 * M17-Power motors (X Y)
 * M18-Disable motors (X Y)
 * M110-Set Current Line Number
 * M111-Enable debug (S-on/off)
 * M112-Emergency stop
 * M114-Get current position
 * M115-Get version
 * # M119-Get endstop status
 * M206-Offset axes (X Y Z)
 *
 *
 * todo
 *  -
 */

Platform platform;
Gcode interpreter(&platform);

void setup() {
	Serial.begin(BAUD_RATE);
}

void loop() {
	platform.Update();
	interpreter.Update();
}
