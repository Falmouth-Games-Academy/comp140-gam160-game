#pragma once
#include "Object.h"
#include "Math.h"
#include "Sprite.h"

class Hand : public Object {
public:
	Hand() {};
	virtual ~Hand();

	void OnSpawn();

	void Update(float deltaTime) override;
	void Render() override;

	Object::Type GetType() const override {return Object::HandType;}

public:
	// Returns a point on the hand, considering its bobbed position
	Vec3 SpritePointToWorldPoint(const Vec2& spritePoint) const override;

public:
	// Gets the direction of the hand (-1 is left, 1 is right)
	inline int GetDirection() const;

	// Returns how much power the laser should have, if any (0 = no laser being fired)
	inline float32 GetLaserPower() const;

	// Returns the exact position of the hand, including its bobbing
	inline Vec3 GetBobbedPosition() const;

private:
	// Sprite for the arm
	Sprite armSprite;

	// Eyes
	class GooglyEye* leftEye = nullptr;
	class GooglyEye* rightEye = nullptr;

	// Offset of the head according to the animated head bob
	Vec2 headBob = Vec2(0.0f, 0.0f);

	// Direction (-1 left, +1 right)
	int32 direction = -1;

	// Power of the laser currently being fired, or 0.0 if no laser is being fired
	float32 laserPower = 0.0f;

	// Time remaining, in seconds, before powerslide ends
	float32 powerslideTimeRemaining = 0.0f;
};

inline int Hand::GetDirection() const {
	return direction;
}

inline float32 Hand::GetLaserPower() const {
	return laserPower;
}

inline Vec3 Hand::GetBobbedPosition() const {
	return position + headBob;
}