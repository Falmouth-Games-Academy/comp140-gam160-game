#pragma once
#include <serial\serial.h>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

class SerialInterface
{
public:
	SerialInterface();
	~SerialInterface();

	bool connect = false;

	void send(std::string msg);
	void getPositions();

	void ledFlash(std::string input);

	void newPos();
	void getButton();

	int getPot1() { return pot1; };
	int getPot2() { return pot2; };

	void close();

	float x = 0;
	float y = 0;

	float maxY = -8;
	float minY = 8;

	float maxX = -6;
	float minX = 6;


	int button1;
	int button2;

	template<typename Out>
	void split(const std::string &s, char delim, Out result) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			*(result++) = item;
		}
	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}

private:
	serial::Serial* mySerial;
	

	int pot1 = 0;
	int pot2 = 0;
};
