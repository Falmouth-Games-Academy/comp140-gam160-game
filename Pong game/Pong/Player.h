#pragma once
#include "Object.h"
#include "SerialInterface.h"

class Player : public Object
{
public:
	void virtual init();
	void virtual Move();
	
	enum Side{Side_Undefined,Side_Left,Side_Right};
	void SetSide(Side eSide);
	SerialInterface* SC;

private:
	Side mySide;
	int speed;
};
