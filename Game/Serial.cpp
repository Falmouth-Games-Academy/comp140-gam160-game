#include "Serial.h"

#include <SetupAPI.h>
#include <devguid.h>

// Interfacing code sourced from Arduino website: https://playground.arduino.cc/Interfacing/CPPWindows
Serial::Serial(const char *portName)
{
    //We're not yet connected
    this->connected = false;

    //Try to connect to the given port throuh CreateFile
    this->hSerial = CreateFile(portName,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

    //Check if the connection was successfull
    if(this->hSerial==INVALID_HANDLE_VALUE)
    {
        //If not success full display an Error
        if(GetLastError()==ERROR_FILE_NOT_FOUND){

            //Print Error if neccessary
            printf("ERROR: Handle was not attached. Reason: %s not available.\n", portName);

        }
        else
        {
            printf("ERROR!!!");
        }
    }
    else
    {
        //If connected we try to set the comm parameters
        DCB dcbSerialParams = {0};

        //Try to get the current
        if (!GetCommState(this->hSerial, &dcbSerialParams))
        {
            //If impossible, show an error
            printf("failed to get current serial parameters!");
        }
        else
        {
            //Define serial connection parameters for the arduino board
            dcbSerialParams.BaudRate=CBR_19200;
            dcbSerialParams.ByteSize=8;
            dcbSerialParams.StopBits=ONESTOPBIT;
            dcbSerialParams.Parity=NOPARITY;
            //Setting the DTR to Control_Enable ensures that the Arduino is properly
            //reset upon establishing a connection
            dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

             //Set the parameters and check for their proper application
             if(!SetCommState(hSerial, &dcbSerialParams))
             {
                printf("ALERT: Could not set Serial Port parameters");
             }
             else
             {
                 //If everything went fine we're connected
                 this->connected = true;
                 //Flush any remaining characters in the buffers 
                 PurgeComm(this->hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
                 //We wait 2s as the arduino board will be reseting
                 Sleep(ARDUINO_WAIT_TIME);
             }
        }
    }

}

Serial::~Serial()
{
    //Check if we are connected before trying to disconnect
    if(this->connected)
    {
        //We're no longer connected
        this->connected = false;
        //Close the serial handler
        CloseHandle(this->hSerial);
    }
}

int Serial::GetNumPendingBytes()
{
    //Use the ClearCommError function to get status info on the Serial port
    ClearCommError(this->hSerial, &this->errors, &this->status);

	return this->status.cbInQue;
}

int Serial::ReadData(char *buffer, unsigned int nbChar)
{
    //Number of bytes we'll have read
    DWORD bytesRead;
    //Number of bytes we'll really ask to read
    unsigned int toRead;

    //Use the ClearCommError function to get status info on the Serial port
    ClearCommError(this->hSerial, &this->errors, &this->status);

    //Check if there is something to read
    if(this->status.cbInQue>0)
    {
        //If there is we check if there is enough data to read the required number
        //of characters, if not we'll read only the available characters to prevent
        //locking of the application.
        if(this->status.cbInQue>nbChar)
        {
            toRead = nbChar;
        }
        else
        {
            toRead = this->status.cbInQue;
        }

        //Try to read the require number of chars, and return the number of read bytes on success
        if(ReadFile(this->hSerial, buffer, toRead, &bytesRead, NULL) )
        {
            return bytesRead;
        }

    }

    //If nothing has been read, or that an error was detected return 0
    return 0;

}


bool Serial::WriteData(const char *buffer, unsigned int nbChar)
{
    DWORD bytesSend;

    //Try to write the buffer on the Serial port
    if(!WriteFile(this->hSerial, (void *)buffer, nbChar, &bytesSend, 0))
    {
        //In case it don't work get comm error and return false
        ClearCommError(this->hSerial, &this->errors, &this->status);

        return false;
    }
    else
        return true;
}

bool Serial::IsConnected()
{
    //Simply return the connection status
    return this->connected;
}

StaticString<128> Serial::GetMostLikelyArduinoPort() {
	const int maxPortNameLength = 128;
	const int maxFriendlyNameLength = 256;
	const int maxHardwareIdLength = 128;

	// Use essentially the boilerplate code to find connected COM devices and their friendly names
	HDEVINFO devInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, nullptr, nullptr, DIGCF_PRESENT);
	SP_DEVINFO_DATA devInfoData;

	devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	// Iterate devices
	for (DWORD memberIndex = 0; SetupDiEnumDeviceInfo(devInfoSet, memberIndex, &devInfoData); ++memberIndex) {
		// Get port name registry key
		HKEY hkey = SetupDiOpenDevRegKey(devInfoSet, &devInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);

		char portName[maxPortNameLength];
		DWORD portNameLength = maxPortNameLength;

		LONG returnCode = RegQueryValueEx(hkey, "PortName", nullptr, nullptr, (LPBYTE)portName, &portNameLength);

		portName[portNameLength - 1] = '\0';
		RegCloseKey(hkey);

		if (returnCode != EXIT_SUCCESS) {
			continue;
		}

		// Ignore parallel ports
		if (strstr(portName, "LPT")) {
			continue;
		}

		// Get the friendly name of the port device
		char friendlyName[maxFriendlyNameLength];
		DWORD friendlyNameLength = 0;

		BOOL wasFriendlyNameFound = SetupDiGetDeviceRegistryProperty(devInfoSet, &devInfoData, SPDRP_FRIENDLYNAME, nullptr, (PBYTE)friendlyName, 
																		maxFriendlyNameLength, &friendlyNameLength);

		if (wasFriendlyNameFound && friendlyNameLength > 0) {
			friendlyName[friendlyNameLength - 1] = '\0';
		} else {
			// We need this name to identify the device
			continue;
		}

		// Check if this is maybe possibly meh who knows the Arduino:
		if (!strncmp(friendlyName, "USB Serial Device", 17)) {
			// OK this is totally 100% guaranteed NO-BS absolutely certainly the Arduino yep
			return portName;
		}
	}

	return "";
}
