#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include  "serial/serial.h"

class SerialController
{
public:
	SerialController();
	~SerialController();

	bool connected = false;

	serial::Serial* arduinoSerial;

	int potentiometer;
	int led;

	int getPlayerPosition(int index);
};

