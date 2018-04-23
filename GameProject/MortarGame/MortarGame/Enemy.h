#pragma once

#include "Object.h"

class Enemy :public Object
{
public:
	Enemy();
	Enemy(const char* texturesheet, int x, int y);


	void Update() override;

	void MoveDown();

	void MoveDown2();

	void MoveDown3();

	void StopMoving();

	void MoveLeft();

	~Enemy();
};

