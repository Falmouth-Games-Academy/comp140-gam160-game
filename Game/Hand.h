#pragma once
#include "Object.h"
#include "Math.h"
#include "Sprite.h"

class Hand : public Object {
public:
	Hand() : position(0.0f, 0.0f), velocity(0.0f, 0.0f), sprite() {};
	~Hand();

	void Spawn();

	void Render();
	void Update(float deltaTime);

	const Vec2& GetPosition() const;

	Object::Type GetType() const override {return Object::Hand;}
private:
	Sprite sprite;

	Vec2 position;
	Vec2 velocity;
	Vec2 headBob;

	int32 direction;

	float32 powerslideTimeRemaining; // Time remaining, in seconds, before powerslide ends
};