#pragma once
#include "Object.h"
#include "Math.h"

class Hand : public Object {
public:
	Hand() : position(0.0f, 0.0f), velocity(0.0f, 0.0f), sprite(nullptr) {};
	~Hand();

	void Spawn();

	void Render();
	void Update(float deltaTime);

	Object::Type GetType() const override {return Object::Hand;}
private:
	struct SDL_Texture* sprite;
	int32 spriteWidth, spriteHeight; // temporary

	Vec2 position;
	Vec2 velocity;

	int32 direction;

	float32 powerslideTimeRemaining; // Time remaining, in seconds, before powerslide ends
};