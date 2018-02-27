#include "Object.h"
#include "Game.h"

void Object::RenderCollisionBox() const {
	SDL_Renderer* renderer = game.GetRenderer(RenderScreen::Main);

	game.GetCamera().RenderRectangle(position - sprite.GetOrigin() + Vec3(collisionBox.position, 0.0f) * sprite.GetScale(), collisionBox.size * sprite.GetScale(), Colour::Blue());
}

bool Object::IsColliding(const Object& otherObject) {
	Vec2 selfOrigin = position.xy - sprite.GetOrigin();
	Vec2 otherOrigin = position.xy - sprite.GetOrigin();
	float selfL = selfOrigin.x + collisionBox.x * sprite.GetScale().x,
		  selfR = selfOrigin.x + (collisionBox.x + collisionBox.width) * sprite.GetScale().x,
		  selfT = selfOrigin.y + collisionBox.y * sprite.GetScale().y,
		  selfB = selfOrigin.y + (collisionBox.y + collisionBox.height) * sprite.GetScale().y;
	float otherL = otherOrigin.x + otherObject.collisionBox.x * sprite.GetScale().x,
		  otherR = otherOrigin.x + (otherObject.collisionBox.x + collisionBox.width) * sprite.GetScale().x,
		  otherT = otherOrigin.y + otherObject.collisionBox.y * sprite.GetScale().y,
		  otherB = otherOrigin.y + (otherObject.collisionBox.y + otherObject.collisionBox.height) * sprite.GetScale().y;

	if (selfL >= otherR || selfR <= otherL || selfT >= otherB || selfB <= otherT) {
		return false;
	} else {
		return true;
	}
}