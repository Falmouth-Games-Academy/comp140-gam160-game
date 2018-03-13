#include "Object.h"
#include "Game.h"

void Object::Update(float deltaTime) {
	// Update gravity
	velocity.y = Math::clamp(velocity.y + 3000.0f * deltaTime, -2000.0f, 6000.0f);
	
	// Move object
	Move(velocity * deltaTime, true);

	// Spin object
	rotation = Math::circleclamp(rotation + rotationSpeed * deltaTime, 360.0f);

	// Countdown destruction timer and destroy self if it reaches 0
	if (destroyTimer > 0.0f) {
		destroyTimer -= deltaTime;

		if (destroyTimer <= 0.0f) {
			Destroy();
		}
	}
}

void Object::Render() {
	game.GetCamera().RenderSprite(sprite, position, rotation);
}

void Object::RenderCollisionBox() const {
	SDL_Renderer* renderer = game.GetRenderer(RenderScreen::Main);
	Vec2 spriteOrigin(0.0f, 0.0f), spriteScale(0.0f, 0.0f);

	if (sprite.GetFrame(0)) {
		spriteOrigin = sprite.GetFrame(0)->GetOrigin();
		spriteScale = sprite.GetFrame(0)->GetScale();
	}

	game.GetCamera().RenderRectangle(position - spriteOrigin + Vec3(collisionBox.position, 0.0f) * spriteScale, collisionBox.size * spriteScale, Colour::Blue());
}

bool Object::IsColliding(const Object& otherObject, Bounds2* borderOffsets) {
	// A collision can only happen if both objects are solid
	if (!(isSolid && otherObject.isSolid)) {
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

	// 
	if (selfL >= otherR || selfR <= otherL || selfT >= otherB || selfB <= otherT) {
		return false;
	} else {
		if (borderOffsets) {
			*borderOffsets = Bounds2(otherR - selfL, otherB - selfT, otherL - selfR, otherT - selfB);
		}

		return true;
	}
}

Vec3 Object::SpritePointToWorldPoint(const Vec2& spritePoint) const {
	if (sprite.GetCurrentFrame()) {
		// Convert the sprite coordinate to a world coordinate
		return position + ((spritePoint - sprite.GetCurrentFrame()->GetBaseOrigin()) * sprite.GetCurrentFrame()->GetScale()).Rotated(rotation);
	} else {
		return position + spritePoint;
	}
}

bool Object::Move(const Vec3& originalMoveOffset, bool doAffectVelocity, bool teleport) {
	// NewMoveOffset
	Vec3 originalPosition = position;
	Vec3 moveOffset(originalMoveOffset.x, 0.0f, originalMoveOffset.z);

	// Perform two movements, first on the X axis and next on the Y axis
	// Very lazy collision system that hurts my brain to know it wouldn't work with any kind of slope
	
	// Move to the desired X position first
	position += moveOffset;

	// Check collisions in this new position and move back if they exist
	Bounds2 pushbackOffsets;
	for (::BackgroundLayer& layer : game.GetLevel().GetLayers()) {
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

	// Now move X+Y
	moveOffset.y = originalMoveOffset.y;
	position = originalPosition + moveOffset;
	
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

	// Done!

	// Stop the object on the offending axis
	if (doAffectVelocity) {
		if (moveOffset.x != originalMoveOffset.x) {
			velocity.x = 0.0f;
		}
		if (moveOffset.y != originalMoveOffset.y) {
			velocity.y = 0.0f;
		}
	}

	return (moveOffset == originalMoveOffset);
}