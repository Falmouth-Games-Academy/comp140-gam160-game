#include "pch.h"
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

void SerialInterface::getInput()
{
	if (connect)
	{
		mySerial->write("I");

		std::string result = mySerial->readline();

		if (result.length() > 22)
		{
			std::string subSpeed = result.substr(0, 4);

			std::string subX = result.substr(5, 9);
			std::string subY = result.substr(10, 14);
			std::string subZ = result.substr(15, 19);

			std::string subState = result.substr(20, 21);

			speed = debounce(0.2, speed, std::stoi(subSpeed));

			prevX = angleX;
			prevY = angleY;
			prevZ = angleZ;

			angleX = debounce(0.2, angleX, std::stoi(subX));
			angleY = debounce(0.2, angleY, std::stoi(subY));
			angleZ = debounce(0.2, angleZ, std::stoi(subZ));

			//std::cout << angleX << " " << angleY << " " << angleZ << "\n";

			state = stoi(subState);

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