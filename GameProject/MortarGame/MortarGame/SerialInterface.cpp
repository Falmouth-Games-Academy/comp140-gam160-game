#include "stdafx.h"
#include "SerialInterface.h"
#include <iostream>
#include <vector>

using std::cout;
using std::vector;
using std::exception;
using std::stoi;
using std::string;


SerialInterface::SerialInterface()
{
	vector <serial::PortInfo> devicesFound = serial::list_ports();

	vector <serial::PortInfo>::iterator iter = devicesFound.begin();

	while (iter != devicesFound.end())
	{
		serial::PortInfo device = *iter++;
		string port = device.port.c_str();

		try
		{
			mySerial = new serial::Serial(port, 115200, serial::Timeout::simpleTimeout(250));

			if (mySerial->isOpen())
			{
				cout << "Connection successful: " << port << "\n";
				connected = true;

				break;
			}
		}
		catch (exception &e)
		{

		}
	}
}


SerialInterface::~SerialInterface()
{
}

void SerialInterface::send(string msg)
{
	if (connected)
	{
		mySerial->write(msg);
	}
}

void SerialInterface::getValues()
{
	if (connected)
	{
		mySerial->write("V");

		string result = mySerial->readline();

		if (result.length() >= 6)
		{
			string sub1 = result.substr(0, 4);
			if (sub1 != "")
			{
				rot1 = std::stoi(sub1);
			}


			string sub2 = result.substr(5, 1);
			if (sub2 != "")
			{
				swtch1 = std::stoi(sub2);
			}


			string sub3 = result.substr(6, 1);
			if (sub3 != "")
			{
				swtch2 = std::stoi(sub3);
			}
		}



	}
}

void SerialInterface::close()
{
	mySerial->flush();
	mySerial->close();
}
