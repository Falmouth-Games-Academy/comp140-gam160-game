#pragma once

#include <serial\serial.h>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

// Data format: PPPP-AAAA-AAAA-AAAA-S
// P - potentiometer reading
// A - gyro rotational axis
// S - state of the button

class SerialInterface
{
public:
	SerialInterface();
	~SerialInterface();

	bool connect = false;

	void send(std::string msg);

	void close();

	void getInput();

	// Debounces Arduino input
	int debounce(float, int, int);

private:
	serial::Serial* mySerial;

public:
	int speed;

	int angleX, angleY, angleZ;

	int prevX, prevY, prevZ;

	int state;
};