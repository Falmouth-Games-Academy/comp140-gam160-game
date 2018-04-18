#pragma once
#include "Object.h"
class Player :	public Object
{
public:
	Player();
	Player(int foo, const char* texturesheet, int x, int y) : Object(texturesheet, x, y) {};
	~Player();
};

