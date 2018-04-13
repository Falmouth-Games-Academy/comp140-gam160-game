#pragma once
#include "Object.h"

class Bottle : public Object {
public:
	void OnSpawn() override;

	void Update(float deltaTime) override;

	void OnDeath() override;

	Object::Type GetType() const override {return BottleType;}

public:
	void Smash(); // SMASH
};

class BottleFragment : public Object {
public:
	void OnSpawn() override;

	Object::Type GetType() const override {return BottleFragmentType;}

public:
	// Sets the piece index of the bottle fragment (same as the sprite index)
	void SetupFragment(const Bottle* parent, int index);
};