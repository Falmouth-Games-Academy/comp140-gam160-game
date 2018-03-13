#pragma once
#include "Object.h"

class Bottle : public Object {
public:
	void Spawn() override;

	void Update(float deltaTime) override;

	Object::Type GetType() const override {return BottleType;}

public:
	void Shatter(); // SMASH

private:
	float32 spinRate;
};

class BottleFragment : public Object {
public:
	void Spawn() override;

	void Update(float deltaTime) override;

	Object::Type GetType() const override {return BottleFragmentType;}

public:
	// Sets the piece index of the bottle fragment (same as the sprite index)
	void SetupFragment(const Bottle* parent, int index);

public:
	float32 spinRate;
};