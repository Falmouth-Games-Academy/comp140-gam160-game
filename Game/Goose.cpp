#include "Goose.h"

void Goose::OnSpawn() {
	// Load the sprite
	sprite.LoadFrames("graphics/enemies/goose/goose", 12, Vec2(1129, 1317), Vec2(0.5f, 0.5f));

	sprite.SetFrameRate(7.0f);
	sprite.SetFrameRange(5, 11);

	// That's all for now I guess!
	updateFlags = UpdateAll & ~UpdateGravity;
}

void Goose::Update(float deltaTime) {
	//sprite.SetFrameRange(0, -1);
	sprite.SetFrameRate(11.0f);

	Object::Update(deltaTime);
}