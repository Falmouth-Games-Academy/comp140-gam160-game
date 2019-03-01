#include "SerialController.h"
#include <iostream>

SerialController::SerialController()
{
	std::vector <serial::PortInfo> devicesFound = serial::list_ports();

	std::vector <serial::PortInfo>::iterator iter = devicesFound.begin();

	while (iter != devicesFound.end())
	{
		serial::PortInfo device = *iter++;
		std::string port = device.port.c_str();

		try {
			arduinoSerial = new serial::Serial(port, 9600, serial::Timeout::simpleTimeout(250));

			if (arduinoSerial->isOpen())
			{
				std::cout << "Connection success: " << port << std::endl;
				connected = true;

				break;
			}
		}
		catch (std::exception &e)
		{

		}
	}
}


SerialController::~SerialController()
{
}

int SerialController::getPlayerPosition(int index)
{
	int currentReading = std::stoi(arduinoSerial->readline());
	return currentReading;
}
