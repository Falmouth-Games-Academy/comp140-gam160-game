#include "Object.h"
#include "Game.h"

void Object::RenderCollisionBox() const {
	SDL_Renderer* renderer = game.GetRenderer(RenderScreen::Main);

	game.GetCamera().RenderRectangle(position - sprite.GetOrigin() + Vec3(collisionBox.position, 0.0f) * sprite.GetScale(), collisionBox.size * sprite.GetScale(), Colour::Blue());
}

bool Object::IsColliding(const Object& otherObject, Bounds2* borderOffsets) {
	// A collision can only happen if both objects are solid
	if (!(isSolid && otherObject.isSolid)) {
		return false;
	}

	// Calculate object  collisionboundaries
	Vec2 selfOrigin = position.xy - sprite.GetOrigin();
	Vec2 otherOrigin = otherObject.position.xy - otherObject.sprite.GetOrigin();
	float selfL = selfOrigin.x + collisionBox.x * sprite.GetScale().x,
		  selfR = selfOrigin.x + (collisionBox.x + collisionBox.width) * sprite.GetScale().x,
		  selfT = selfOrigin.y + collisionBox.y * sprite.GetScale().y,
		  selfB = selfOrigin.y + (collisionBox.y + collisionBox.height) * sprite.GetScale().y;
	float otherL = otherOrigin.x + otherObject.collisionBox.x * otherObject.sprite.GetScale().x,
		  otherR = otherOrigin.x + (otherObject.collisionBox.x + collisionBox.width) * otherObject.sprite.GetScale().x,
		  otherT = otherOrigin.y + otherObject.collisionBox.y * otherObject.sprite.GetScale().y,
		  otherB = otherOrigin.y + (otherObject.collisionBox.y + otherObject.collisionBox.height) * otherObject.sprite.GetScale().y;

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

bool Object::Move(const Vec3& originalMoveOffset, bool teleport) {
	// NewMoveOffset
	Vec3 moveOffset = originalMoveOffset;

	// Move to the desired position first
	position += originalMoveOffset;

	// Find out how far we overstepped
	// Find other objects and background layers that might get in the way
	Bounds2 pushbackOffsets;
	for (::BackgroundLayer& layer : game.GetLevel().GetLayers()) {
		if (IsColliding(layer, &pushbackOffsets)) {
			// Which direction are we most likely to push back in?
			if (abs(moveOffset.y) > abs(moveOffset.x)) {
				// Vertically
				if (moveOffset.y > 0.0f) {
					moveOffset.y += pushbackOffsets.bottom;
				} else {
					moveOffset.y += pushbackOffsets.top;
				}
			} else {
				// Horizontally
				if (moveOffset.x > 0.0f) {
					moveOffset.x += pushbackOffsets.right;
				} else {
					moveOffset.x += pushbackOffsets.left;
				}
			}
		}
	}

	// Then move back again, and move forward by the new adjusted vector
	position -= originalMoveOffset;

	// Perform the movement!
	position += moveOffset;

	return (moveOffset == originalMoveOffset);
}