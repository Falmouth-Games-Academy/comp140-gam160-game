#pragma once
#include "Object.h"
#include "Math.h"
#include "Sprite.h"

class Hand : public Object {
public:
	Hand() : headBob(0.0f, 0.0f), powerslideTimeRemaining(0.0f), direction(-1) {};
	virtual ~Hand();

	void Spawn();

	void Update(float deltaTime) override;
	void Render() override;

	Object::Type GetType() const override {return Object::HandType;}

public:
	// Gets the direction of the hand (-1 is left, 1 is right)
	inline int GetDirection() const;

	// Returns how much power the laser should have, if any (0 = no laser being fired)
	inline float32 GetLaserPower() const;

private:
	// Offset of the head according to the animated head bob
	Vec2 headBob;

	// Direction (-1 left, +1 right)
	int32 direction;

	// Power of the laser currently being fired, or 0.0 if no laser is being fired
	float32 laserPower;

	// Time remaining, in seconds, before powerslide ends
	float32 powerslideTimeRemaining;
};

inline int Hand::GetDirection() const {
	return direction;
}

inline float32 Hand::GetLaserPower() const {
	return laserPower;
}