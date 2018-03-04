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

	// Gets the direction of the hand (-1 is left, 1 is right)
	inline int GetDirection() const;
private:
	Vec2 headBob;

	int32 direction;

	float32 powerslideTimeRemaining; // Time remaining, in seconds, before powerslide ends
};

inline int Hand::GetDirection() const {
	return direction;
}