#pragma once
class Player;
class Input;
class Graphics;

#include "serialInterface.h"
#include "serverConnect.h"

class Controls{
public:

	Controls();
	~Controls();

	SerialInterface serial;
	ServerConnect server;

	void playerControls(Player &player, Input &input, Graphics &graphics);

	bool controllerActive = false;
	bool serverActive = false;

private:

};