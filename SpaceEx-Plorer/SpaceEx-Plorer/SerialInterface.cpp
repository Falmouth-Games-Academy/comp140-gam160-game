#include "stdafx.h"
#include "SerialInterface.h"
#include <iostream>

SerialInterface::SerialInterface()
{
	std::vector <serial::PortInfo> devicesFound = serial::list_ports();

	std::vector <serial::PortInfo>::iterator iter = devicesFound.begin();

	while (iter != devicesFound.end())
	{
		serial::PortInfo device = *iter++;
		std::string port = device.port.c_str();

		try {
			mySerial = new serial::Serial(port, 9600, serial::Timeout::simpleTimeout(250));		// Remember to change speed if needed (Boardrate)

			if (mySerial->isOpen())
			{
				std::cout << "Connection success: " << port << "\n";
				connect = true;

				break;
			}
		}
		catch (std::exception &e) {

		}
	}
}
SerialInterface::~SerialInterface()
{

}

void SerialInterface::send(std::string msg)
{
	if (connect)
	{
		mySerial->write(msg);
	}
}

// 1 for player one; -1 for player two
void SerialInterface::score(int player)
{
	switch (player)
	{
	case -1: { mySerial->write("L"); break; }
	case 1: { mySerial->write("R"); break; }
	default: {break; }
	}
}

void SerialInterface::resetGame()
{
	mySerial->write("Q");
}

void SerialInterface::getButton()
{
	if (connect)
	{
		mySerial->write("B");

		std::string result = mySerial->readline();

		//std::vector<std::string> pos = split(result, ';');

		std::string b1 = result.substr(0, 1);
		std::string b2 = result.substr(2, 1);

		button1 = std::stoi(b1);
		button2 = std::stoi(b2);
		//std::cout << button1 << std::endl;
	}
}

void SerialInterface::getPositions()
{
	if (connect)
	{
		mySerial->write("P");

		std::string result = mySerial->readline();


		if (result.length() > 5) {
			std::string sub1 = result.substr(0, 4);

			std::string sub2 = result.substr(5, 9);

			pot1 = debounce(0.2, pot1, std::stoi(sub1));
			pot2 = debounce(0.2, pot2, std::stoi(sub2));
		}

	}
}

int SerialInterface::debounce(float factor, int oldValue, int newValue)
{
	return (1.0 - factor) * oldValue + factor * newValue;
}

void SerialInterface::close()
{
	mySerial->flush();
	mySerial->close();
}