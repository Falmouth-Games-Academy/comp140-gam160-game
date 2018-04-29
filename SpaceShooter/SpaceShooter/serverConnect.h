#ifndef SERVERCONNECT_H
#define SERVERCONNECT_H

#include <iostream>
#include <string>
#include "sio_client.h"
#include "sio_message.h"
#include "sio_socket.h"
#include "connectionListener.h"


class ServerConnect {
public:
	void connectToServer(std::string serverIP);

	bool isConnected()
	{
		return h.opened();
	}

	void onMessage();


private:
	sio::client h;
	ConnectionListener * l;


};
#endif