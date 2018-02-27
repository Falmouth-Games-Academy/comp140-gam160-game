#pragma once
#include "Types.h"

#pragma pack(push, 1)
struct ArduinoToPCData {
	// Start data stream marker
	uint16 startMarker; //=0xADDE

	// Accelerometer values
	int16 accelX;
	int16 accelY;
	int16 accelZ;

	// Raw bend sensor value between 0 and 1024
	uint16 flexValue;

	// End data stream marker
	uint16 endMarker; //=0x0DD0
};
#pragma pack(pop)