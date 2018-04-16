#include "Object.h"
#include "Game.h"

Object::SpawnerDatabase Object::spawners;

void Object::OnSpawn() {
	return;
}

void Object::OnDestroy() {
	return;
}

void Object::Update(float deltaTime) {
	if (updateFlags & UpdateGravity) {
		// Update gravity
		velocity.y = Math::clamp(velocity.y + game.GetGravity() * deltaTime, -40000.0f, 40000.0f);
	}
	
	if (updateFlags & UpdatePhysics) {
		Vec3 expectedPosition = position + velocity * deltaTime;

		// Move object
		bool wasMoveUnobstructed = Move(velocity * deltaTime, true);

		// Check if the object has landed on the ground
		if (!wasMoveUnobstructed && position.y < expectedPosition.y) {
			isOnGround = true;
		} else if (wasMoveUnobstructed && velocity.y != 0.0f) {
			isOnGround = false;
		}
	}

	if (updateFlags & UpdateRotation) {
		// Spin object
		rotation = Math::circleclamp(rotation + rotationSpeed * deltaTime, 360.0f);
	}

	if (updateFlags & UpdateDeathTimer) {
		// Countdown destruction timer and destroy self if it reaches 0
		if (destroyTimer > 0.0f) {
			destroyTimer -= deltaTime;

			if (destroyTimer <= 0.0f) {
				Destroy();
			}
		}
	}

	if (updateFlags & UpdateHurtFlashes) {
		// Countdown visual hurt flash timer
		if (hurtFlashTimer > 0.0f) {
			hurtFlashTimer -= deltaTime;

			Colour blendColour = Colour::Red();
			blendColour.g = blendColour.b = (uint8)((maxHurtFlashTimer - hurtFlashTimer) * 255.0f / maxHurtFlashTimer);
			sprite.SetBlendColour(blendColour);
		} else {
			sprite.SetBlendColour(Colour::White());
		}
	}

	if (updateFlags & UpdateInvincibilityTimer) {
		if (hurtInvincibilityTimer > 0.0f) {
			hurtInvincibilityTimer -= deltaTime;
		}
	}

	if (updateFlags & UpdateAnimation) {
		sprite.TickAnimation(deltaTime);
	}
}

void Object::Render() {
	game.GetCamera().RenderSprite(sprite, position, rotation);
}

void Object::OnDamage() {
}

void Object::RenderCollisionBox() const {
	SDL_Renderer* renderer = game.GetSDLRenderer(RenderScreen::Main);
	Vec2 spriteOrigin(0.0f, 0.0f), spriteScale(0.0f, 0.0f);

	if (sprite.GetFrame(0)) {
		spriteOrigin = sprite.GetFrame(0)->GetOrigin();
		spriteScale = sprite.GetFrame(0)->GetScale();
	}

	game.GetCamera().RenderRectangle(position - spriteOrigin + Vec3(collisionBox.position, 0.0f) * spriteScale, collisionBox.size * spriteScale, Colour::Blue());
}

bool Object::IsColliding(const Object& otherObject, Bounds2* borderOffsets) {
	// A collision can only happen if both objects are solid
	if (!((collisionFlags & (SolidObjs | OverlapObjs)) && (otherObject.collisionFlags & (SolidObjs | OverlapObjs)))) {
		return false;
	}

	Vec2 spriteOrigin(0.0f, 0.0f), spriteScale(0.0f, 0.0f);
	Vec2 otherSpriteOrigin(0.0f, 0.0f), otherSpriteScale(0.0f, 0.0f);

	// Get sprite frame info
	if (sprite.GetFrame(0)) {
		spriteOrigin = sprite.GetFrame(0)->GetOrigin();
		spriteScale = sprite.GetFrame(0)->GetScale();
	}

	if (otherObject.sprite.GetFrame(0)) {
		otherSpriteOrigin = otherObject.sprite.GetFrame(0)->GetOrigin();
		otherSpriteScale = otherObject.sprite.GetFrame(0)->GetScale();
	}

	// Calculate object collision boundaries
	Vec2 selfOrigin = position.xy - spriteOrigin;
	Vec2 otherOrigin = otherObject.position.xy - otherSpriteOrigin;
	float selfL = selfOrigin.x + collisionBox.x * spriteScale.x,
		  selfR = selfOrigin.x + (collisionBox.x + collisionBox.width) * spriteScale.x,
		  selfT = selfOrigin.y + collisionBox.y * spriteScale.y,
		  selfB = selfOrigin.y + (collisionBox.y + collisionBox.height) * spriteScale.y;
	float otherL = otherOrigin.x + otherObject.collisionBox.x * otherSpriteScale.x,
		  otherR = otherOrigin.x + (otherObject.collisionBox.x + otherObject.collisionBox.width) * otherSpriteScale.x,
		  otherT = otherOrigin.y + otherObject.collisionBox.y * otherSpriteScale.y,
		  otherB = otherOrigin.y + (otherObject.collisionBox.y + otherObject.collisionBox.height) * otherSpriteScale.y;

	// Check if there's a collision
	if (selfL >= otherR || selfR <= otherL || selfT >= otherB || selfB <= otherT) {
		return false;
	} else {
		if (borderOffsets) {
			*borderOffsets = Bounds2(otherR - (selfL - 0.1f), otherB - (selfT - 0.1f), otherL - (selfR + 0.1f), otherT - (selfB + 0.1f)); // the 0.1fs are for precision
		}

		return true;
	}
}

void Object::ChangeHealth(float32 healthDelta, float32 hurtInvincibilityTime) {
	if ((hurtInvincibilityTimer > 0.0f && healthDelta < 0.0f) || !isHurtable) {
		return; // You can't hurt me while I'm invincible, fool!
	}

	// Affect health
	health = Math::clamp(health + healthDelta, 0.0f, 100.0f);

	// Die if dead
	if (health <= 0.0f) {
		OnDeath();
		return;
	}

	// Do flash visual feedback
	hurtFlashTimer = maxHurtFlashTimer;

	// Apply invincibility
	if (hurtInvincibilityTime != -1.0f) {
		hurtInvincibilityTimer = hurtInvincibilityTime;
	} else {
		hurtInvincibilityTimer = defaultHurtInvincibilityTime;
	}
}

Vec3 Object::SpritePointToWorldPoint(const Vec2& spritePoint) const {
	if (sprite.GetCurrentFrame()) {
		// Convert the sprite coordinate to a world coordinate
		return sprite.GetCurrentFrame()->PixelToWorld(spritePoint, position, rotation);
	} else {
		return position + spritePoint;
	}
}

void Object::OnOverlap(Object& otherObject) {
	// Compute the answer to life the universe and everything
	return;
}

void Object::OnDeath() {
	Destroy();
}

bool Object::Move(const Vec3& originalMoveOffset, bool teleport) {
	// NewMoveOffset
	Vec3 originalPosition = position;
	Vec3 moveOffset(originalMoveOffset.x, 0.0f, originalMoveOffset.z);
	Bounds2 pushbackOffsets;

	// Perform two movements, first on the X axis and next on the Y axis
	// Very lazy collision system that hurts my brain to know it wouldn't work with any kind of slope
	
	// Move to the desired X position first
	position += moveOffset;

	if (collisionFlags & SolidEnv) {
		// Check collisions in this new position and move back if they exist
		for (BackgroundLayer& layer : game.GetLevel().GetLayers()) {
			if (IsColliding(layer, &pushbackOffsets)) {
				// Push back on X axis
				if (moveOffset.x > 0.0f) {
					moveOffset.x += pushbackOffsets.right;
				} else {
					moveOffset.x += pushbackOffsets.left;
				}

				position = originalPosition + moveOffset;
			}
		}
	}

	// Now move X+Y
	moveOffset.y = originalMoveOffset.y;
	position = originalPosition + moveOffset;
	
	if (collisionFlags & SolidEnv) {
		for (::BackgroundLayer& layer : game.GetLevel().GetLayers()) {
			if (IsColliding(layer, &pushbackOffsets)) {
				// Push back on X axis
				if (moveOffset.y > 0.0f) {
					moveOffset.y += pushbackOffsets.bottom;
				} else {
					moveOffset.y += pushbackOffsets.top;
				}

				position = originalPosition + moveOffset;
			}
		}
	}

	// Do overlap events
	if (collisionFlags & OverlapObjs) {
		for (Object* object : game.GetObjects()) {
			if (IsColliding(*object)) {
				// Hack: check if it was colliding before
				position = originalPosition;
				bool wasColliding = IsColliding(*object);
				position = originalPosition + moveOffset;

				if (!wasColliding) {
					// Call the overlap events
					object->OnOverlap(*this);
					this->OnOverlap(*object);
				}
			}
		}
	}

	// Stop the object on the offending axis
	if (!(collisionFlags & PreserveXVelocity) && moveOffset.x != originalMoveOffset.x) {
		velocity.x = 0.0f;
	}
	if (!(collisionFlags & PreserveYVelocity) && moveOffset.y != originalMoveOffset.y) {
		velocity.y = 0.0f;
	}

	return (moveOffset == originalMoveOffset);
}

#include "Bottle.h"
#include "GooglyEye.h"
#include "Laser.h"
#include "Object.h"
#include "Bigfoot.h"
#include "Goose.h"

Object::SpawnerDatabase::SpawnerDatabase() {
	// Type definitions go here!
#define SET_SPAWNER(TypeName) spawners[TypeName##Type] = [](){return (Object*)new TypeName;};

	SET_SPAWNER(Bottle);
	SET_SPAWNER(Laser);
	SET_SPAWNER(GooglyEye);
	SET_SPAWNER(Hand);
	SET_SPAWNER(Bigfoot);
	SET_SPAWNER(Goose);
}
