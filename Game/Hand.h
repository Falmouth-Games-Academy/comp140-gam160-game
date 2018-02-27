#pragma once
#include "Object.h"
#include "Math.h"
#include "Sprite.h"

class Hand : public Object {
public:
	Hand() {};
	~Hand();

	void Spawn();

	void Render();
	void Update(float deltaTime);

	Object::Type GetType() const override {return Object::Hand;}
private:
	Vec2 headBob;

	int32 direction;

	float32 powerslideTimeRemaining; // Time remaining, in seconds, before powerslide ends
};