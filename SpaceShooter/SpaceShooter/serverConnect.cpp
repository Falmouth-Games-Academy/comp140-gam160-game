#include "serverConnect.h"
#include <iostream>

void ServerConnect::connectToServer(std::string serverIP)
{
	//l = new ConnectionListener(h);
	//h.set_open_listener(std::bind(&ConnectionListener::onConnected, l));
	h.connect(serverIP);


	if (h.opened() == true)
	{
		std::cout << "Connected to server!" << std::endl;
	}

	//h.socket()->emit("Connected!");
	//sio::socket::event_listener();

	//h.socket->on("spaceship", sio::socket::event_listener([&socket, &line](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::ptr &ack_resp){}));
}

void ServerConnect::onMessage()
{
	std::cout << "Message has gone through " << std::endl;
}