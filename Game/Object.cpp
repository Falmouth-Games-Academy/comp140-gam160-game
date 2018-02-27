#include "Object.h"
#include "Game.h"

void Object::RenderCollisionBox() const {
	SDL_Renderer* renderer = game.GetRenderer(RenderScreen::Main);

	game.GetCamera().RenderRectangle(position + Vec3(collisionBox.position, 0.0f) * sprite.GetScale(), collisionBox.size * sprite.GetScale(), Colour::Blue());
}

bool Object::IsColliding(const Object& otherObject) {
	float selfL = position.x + collisionBox.x * sprite.GetScale().x,
		  selfR = position.x + (collisionBox.x + collisionBox.width) * sprite.GetScale().x,
		  selfT = position.y + collisionBox.y * sprite.GetScale().y,
		  selfB = position.y + (collisionBox.y + collisionBox.height) * sprite.GetScale().y;
	float otherL = otherObject.position.x + otherObject.collisionBox.x * sprite.GetScale().x,
		  otherR = otherObject.position.x + (otherObject.collisionBox.x + collisionBox.width) * sprite.GetScale().x,
		  otherT = otherObject.position.y + otherObject.collisionBox.y * sprite.GetScale().y,
		  otherB = otherObject.position.y + (otherObject.collisionBox.y + otherObject.collisionBox.height) * sprite.GetScale().y;

	if (selfL >= otherR || selfR <= otherL || selfT >= otherB || selfB <= otherT) {
		return false;
	} else {
		return true;
	}
}