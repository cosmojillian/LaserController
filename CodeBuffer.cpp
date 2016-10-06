/*
 * CodeBuffer.cpp
 *
 *  Created on: Sep 28, 2016
 *      Author: cosmo
 */

#include "CodeBuffer.h"

CodeBuffer::CodeBuffer() {
	readPointer = 0;
	writePointer = 0;
	bufferLength = 0;
	linePointer = -1;
	commandIndex = 0;
	inComment = false;
}

long CodeBuffer::GetLongValue() {
	if(linePointer < 0) {
		linePointer = -1;
		return 0;
	}

	long result = strtol(&codeBuffer[readPointer][linePointer + 1], NULL, 0);
	linePointer = -1;
	return result;
}

float CodeBuffer::GetFloatValue() {
	if(linePointer < 0) {
		linePointer = -1;
		return 0;
	}

	float result = (float)strtod(&codeBuffer[readPointer][linePointer + 1], NULL);
	linePointer = -1;
	return result;
}

bool CodeBuffer::Seen(char c) {
	linePointer = 0;
	char b;

	do {
		b = codeBuffer[readPointer][linePointer];
		if (b == c) return true;
		linePointer++;
	} while (b != 0 && b != ';');

	linePointer = -1;
	return false;
}

bool CodeBuffer::QueueCommand(const char* command) {
	if (bufferLength > BUFFER_LENGTH)
		return false;
	strcpy(codeBuffer[writePointer], command);
	writePointer = (writePointer + 1) % BUFFER_LENGTH;
	bufferLength++;
	return true;
}

void CodeBuffer::CommandExecuted() {
	readPointer = (readPointer + 1) % BUFFER_LENGTH;
	bufferLength--;
}

void CodeBuffer::FillCodeBuffer() {
	while (bufferLength < BUFFER_LENGTH && Serial.available() > 0) {
		char c = Serial.read();			//the next char for the command

		if (c == '\n' || c == '\r') {
			//End of line
			if (!commandIndex) //if the command index is at 0 then the command is empty and we should skip it
				continue;

			inComment = false;
			newCommand[commandIndex] = 0;
			commandIndex = 0;

			QueueCommand(newCommand);
		} else if (commandIndex >= MAX_COMMAND_LENGTH) {
			//ignore characters past, maybe send an error code?
		} else {							//Add everything else to the new command
			if (c == ';')
				inComment = true;
			if (!inComment)
				newCommand[commandIndex++] = c;
		}
	}
}

char* CodeBuffer::CurrentCode() {
	return codeBuffer[readPointer];
}

bool CodeBuffer::IsEmpty() const {
	return bufferLength == 0;
}

