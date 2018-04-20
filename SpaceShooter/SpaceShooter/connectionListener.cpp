#include "connectionListener.h"
#include <iostream>

ConnectionListener::ConnectionListener(sio::client& h) :
	handler(h)
{
	connectFinish = false;
}

void ConnectionListener::onConnected()
{

	connectFinish = true;
	std::cout << "It COnnected YAY!" << std::endl;
}

void ConnectionListener::onClose(sio::client::close_reason const &reason)
{
	std::cout << "Connection has been closed" << std::endl;
}

void ConnectionListener::onFail()
{
	std::cout << "Connection has failed" << std::endl;
}
