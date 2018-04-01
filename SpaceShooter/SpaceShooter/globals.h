#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

namespace globals
{
	const int SCREEN_WIDTH = 600;
	const int SCREEN_HEIGHT = 780;

	const float SPRITE_SCALE = 1.0f;
}

enum Direction
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct Vector2
{
	int x, y;
	Vector2() :
		x(), y()
	{}
	Vector2(int x, int y) :
		x(x), y(y)
	{}
	Vector2 zero()
	{
		return Vector2(0, 0);
	}
};

#endif
