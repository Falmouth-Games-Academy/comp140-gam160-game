#include "stdafx.h"
#include "SerialInterface.h"
#include <iostream>
#include <vector>

using std::cout;
using std::vector;
using std::exception;
using std::stoi;
using std::string;

const int RotStartIndex = 0;
const int RotDataSize = 4;
const int Swtch1StartIndex = 4;
const int Swtch1DataSize = 1;
const int Swtch2StartIndex = 5;
const int Swtch2DataSize = 1;


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
			mySerial = new serial::Serial("COM3", 115200, serial::Timeout::simpleTimeout(250));

			if (mySerial->isOpen())
			{
				cout << "Connection successful: " << port << "\n";
				connected = true;
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

/* break the string of numbers recieved from the arduino down to 3 substrings, one for the
rotary encoder, one for the lever switch and one for the slider switch, then convert these to 
integers that can be used to move the player */

void SerialInterface::getValues()
{
	if (connected)
	{
		mySerial->write("V");

		string result = mySerial->readline();

		if (result.length() >= 6)
		{
			string sub1 = result.substr(RotStartIndex, RotDataSize);
			if (sub1 != "")
			{
				rot1 = std::stoi(sub1);
			}


			string sub2 = result.substr(Swtch1StartIndex, Swtch1DataSize);
			if (sub2 != "")
			{
				swtch1 = std::stoi(sub2);
			}


			string sub3 = result.substr(Swtch2StartIndex, Swtch2DataSize);
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
