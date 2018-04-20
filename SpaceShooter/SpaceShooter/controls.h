#pragma once
class Player;
class Input;

#include "serialInterface.h"
#include "serverConnect.h"

class Controls{
public:

	Controls();
	~Controls();

	SerialInterface serial;
	ServerConnect server;

	void playerControls(Player &player, Input &input);

	bool controllerActive = false;
	bool serverActive = false;

private:

};