#pragma once
#include "Object.h"

class Bottle : public Object {
public:
	void OnSpawn() override;

	void Update(float deltaTime) override;

	void OnDeath() override;

	Object::Type GetType() const override {return BottleType;}

public:
	void Toss(Vec3 force); // Tosses the bottle in a certain direction

	void Smash(); // SMASH

public:
	float32 tossScaleRate = 1.0f;

	float32 defaultScale = 0.3f;
};

class BottleFragment : public Object {
public:
	void OnSpawn() override;

	Object::Type GetType() const override {return BottleFragmentType;}

public:
	// Sets the piece index of the bottle fragment (same as the sprite index)
	void SetupFragment(const Bottle* parent, int index);
};