#pragma once
#include "Object.h"
class Enemy : public Object
{
public:
	Enemy();
	Enemy(const char* texturesheet, int x, int y) : Object(texturesheet, x, y) {};


	void Update() override;

	void MoveDown();

	void MoveLeft();

	~Enemy();
};

