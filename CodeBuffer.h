/*
 * CodeBuffer.h
 *
 *  Created on: Sep 28, 2016
 *      Author: cosmo
 */

#ifndef CODEBUFFER_H_
#define CODEBUFFER_H_

#include "LaserController.h"

class CodeBuffer {
public:
	CodeBuffer();					//
	//void Init();					//
	bool IsEmpty() const;			//
	bool Seen(char c);				//Searches for c in the codeBuffer at readPointer and sets linePointer to it's index
	float GetFloatValue();			//Returns the float value following linePointer in the code buffer
	long GetLongValue();			//Returns the long value following linePointer in the code buffer
	int GetIntValue();				//Returns the int value following linePointer in the code buffer
	//const char* Buffer() const;		//
	void FillCodeBuffer();
	char* CurrentCode();
	bool QueueCommand(const char* command);
	void CommandExecuted();

private:
	char codeBuffer[BUFFER_LENGTH][MAX_COMMAND_LENGTH];			//The Gcode
	uint8_t readPointer;		//Where in the buffer we need to read next
	uint8_t linePointer;		//Where in the line to read from (for the Get"X" functions)
	uint8_t writePointer;		//Where in the buffer we will write next
	uint8_t bufferLength;		//How long the buffer is
	int commandIndex;

	char newCommand[MAX_COMMAND_LENGTH];
	bool inComment;

	/**
	 * Marlin loops through the buffer, so 0 isnt the "first" command.
	 * FillCodeBuffer reads code from serial if we have room in the buffer
	 */
};

inline int CodeBuffer::GetIntValue() {
	return static_cast<int>(GetLongValue());
}

/*inline const char* CodeBuffer::Buffer() const {
	return codeBuffer;
}*/

#endif /* CODEBUFFER_H_ */
