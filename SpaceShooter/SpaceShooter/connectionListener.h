#pragma once
#include "sio_client.h"
#include <mutex>
#include <condition_variable>

//static 
//static std::mutex _lock;
//static std::condition_variable _cond;

class ConnectionListener
{
	sio::client &handler;

public:
	ConnectionListener(sio::client& h);
	void onConnected();
	void onClose(sio::client::close_reason const &reason);
	void onFail();

	bool connectFinish;
};