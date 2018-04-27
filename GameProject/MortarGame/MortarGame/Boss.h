#pragma once
#include "Object.h"


class Boss :public Object
{
public:
	Boss();
	Boss(const char* texturesheet, float x, float y);


	void Update() override;

	void MoveDown(float speed);

	void StopMoving();

	~Boss();
};

