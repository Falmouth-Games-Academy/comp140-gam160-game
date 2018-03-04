#pragma once
#include "Object.h"

class Laser : public Object {
public:
	Laser() : width(100.0f), baseColour(Colour::Red()) {};

	void Spawn() override;

	void Update(float deltaTime) override;
	void Render() override;

	Object::Type GetType() const override {
		return LaserType;
	}

private:
	// Primary colour of the laser
	Colour baseColour;

	// Average width of the laser, in pixels
	float32 width;

	// All sprites
	enum SpriteFrame {
		CentreFiery = 0,
		EdgeFlames = 1,
		OverLightningStart = 2,
		OverLightningEnd = 7,
		UnderLightning = 8,
		LightBall = 9,
		NumSpriteFrames,
	};

	Sprite spriteFrames[NumSpriteFrames];
};