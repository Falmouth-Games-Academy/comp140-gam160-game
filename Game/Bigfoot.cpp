#include "Bigfoot.h"
#include "Game.h"

void Bigfoot::OnSpawn() {
	// Load Bigfoot sprite
	sprite.LoadFrame("./graphics/enemies/bigfoot/bigfoot.png", Vec2(1707.0f, 5291.0f));

	// Set collision boundary
	collisionBox = Rect2(433.0f, 9.0f, 2401.0f, 5289.0f);
	collisionFlags = SolidEnv | OverlapObjs | PreserveXVelocity;
	
	// Misc init
	isHurtable = true;

	health = initialHealth;

	// Set update flags
	updateFlags = UpdateAll;

	// Prepare to jump!!
	jumpTimer = secsPerJump;
}

void Bigfoot::Update(float deltaTime) {
	const float groundY = 2000.0f;

	if (jumpTimer >= 0.0f) {
		jumpTimer -= deltaTime;

		if (jumpTimer < 0.0f) {
			Jump();
		}
	}

	// Update simulation
	bool wasOnGround = isOnGround;

	Object::Update(deltaTime);

	// Don't die tho
	if (position.y > groundY) {
		position.y = groundY;
		velocity.y = 0.0f;
		isOnGround = true;
	}

	// Handle landings
	if (isOnGround && !wasOnGround) {
		const float32 shakeMaxRange = 3000.0f; // Max range from which the player will notice a camera shake
		const float32 shakeMaxMagnitude = 1000.0f; // Shake magnitude at the closest range

		// Shake the camera when landing. Boom!!
		game.GetCamera().StartShake(0.75f, 18.0f, 
			Math::clamp((shakeMaxRange - Vec3::Distance(position, game.GetCamera().GetPosition())) / shakeMaxRange * shakeMaxMagnitude, 0.0f, shakeMaxMagnitude)
				* (health / initialHealth));

		// Get ready for the next jump
		jumpTimer = secsPerJump;
		velocity.x = 0.0f;
	}

	// Shrink according to health
	sprite.SetScale(Vec2(health / initialHealth));

	// If close enough to the player, request camera focus
	game.GetCamera().AddViewTarget(position, GetSize() * Vec2(0.4f, 0.2f));
}

void Bigfoot::Jump() {
	// Jump high enough to remain in the air for airTime
	velocity.y = -game.GetGravity() * airTime * 0.5f;

	// Jump toward the player
	velocity.x = (game.GetPlayer().GetPosition().x - position.x) / airTime;

	// Cap jump distance
	if (abs(velocity.x * airTime) >= maxJumpDistance) {
		velocity.x = Math::getsign(velocity.x) * maxJumpDistance / airTime;
	}
}

void Bigfoot::OnOverlap(Object& other) {
	// Hurt the player when stomping (but hurt them less if we're unhealthy)
	if (velocity.y > 0.0f && other.GetType() == HandType) {
		other.ChangeHealth(-25.0f * (health / initialHealth));
	}
}
