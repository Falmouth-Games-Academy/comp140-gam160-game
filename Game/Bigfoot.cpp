#include "Bigfoot.h"
#include "Game.h"

void Bigfoot::OnSpawn() {
	// Load Bigfoot sprite
	sprite.LoadFrame("./graphics/enemies/bigfoot/bigfoot.png", Vec2(1707.0f, 5291.0f));

	jumpTimer = 1.0f;
}

void Bigfoot::Update(float deltaTime) {
	const float groundY = 950.0f;

	// Update simulation (this should really be done in Objects)
	bool wasOnGround = isOnGround;

	if (position.y > groundY) {
		if (!isOnGround) {
			jumpTimer = secsPerJump;
			velocity.x = 0.0f;
		}

		isOnGround = true;

		position.y = groundY;
		velocity.y = 0.0f;
	} else {
		isOnGround = false;
	}

	if (isOnGround && !wasOnGround) {
		// Shake the camera when landing. Boom!!
		game.GetCamera().StartShake(0.75f, 18.0f, 1000.0f);
	}

	if (jumpTimer >= 0.0f) {
		jumpTimer -= deltaTime;

		if (jumpTimer < 0.0f) {
			Jump();
		}
	}

	// Do gravity (if not on ground)
	velocity.y += game.GetGravity() * deltaTime;

	// Move
	Move(velocity * deltaTime);
}

void Bigfoot::Jump() {
	// Jump high enough to remain in the air for airTime
	velocity.y = -game.GetGravity() * airTime * 0.5f;

	// Jump toward the player
	velocity.x = (game.GetPlayer().GetPosition().x - position.x) / airTime;
}