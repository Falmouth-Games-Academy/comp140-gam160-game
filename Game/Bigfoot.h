#pragma once

#include "Object.h"

class Bigfoot : public Object {
public:
	void OnSpawn() override;

	void Update(float deltaTime) override;

	Object::Type GetType() const override {return BigfootType;}

public:
	void Jump();

	void OnOverlap(Object& other) override;

private:
	enum BigfootState : uint8 {
		Waiting = 0,
		Jumping
	};

	BigfootState state;

	// Time until the next jump, in sec, or <0 if disabled
	float32 jumpTimer;

	// Speed of the jump cycle
	float32 secsPerJump = 0.5f;

	// Amount of time in the air per jump
	float32 airTime = 1.0f;

	// Maximum distance, in pixels, that BigFoot can jump towrad the player
	float32 maxJumpDistance = 2000.0f;
};