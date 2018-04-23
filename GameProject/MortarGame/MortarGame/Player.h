#pragma once
#include "Object.h"
class Player : public Object
{
public:
	Player();
	Player(const char* texturesheet, int x, int y) : Object(texturesheet, x, y) {};

	void Update() override;

	bool hasFired();

	~Player();
};

