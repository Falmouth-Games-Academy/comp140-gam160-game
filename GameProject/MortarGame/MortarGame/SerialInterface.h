#pragma once
#include <serial\serial.h>
#include <string>

using std::string;

class SerialInterface
{
public:
	SerialInterface();
	~SerialInterface();

	void send(string msg);
	void getValues();

	int getRot1() { return rot1; };
	int getSwtch1() { return swtch1; };
	int getSwtch2() { return swtch2; };

	void close();

private:
	serial::Serial* mySerial;
	bool connected = false;

	int rot1 = 5000;
	int swtch1 = 0;
	int swtch2 = 0;
};

