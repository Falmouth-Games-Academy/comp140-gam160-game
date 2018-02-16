#include "Camera.h"
#include "Game.h"

void Camera::Update() {
	// Follow the player
	position.xy = game.GetPlayer().GetPosition();
	//position.z = 0.0f;

	// Get the viewport size
	SDL_Rect renderViewport;
	SDL_RenderGetViewport(game.GetRenderer(), &renderViewport);

	// Update the camera viewport size based on scaling etc
	float zScale = 0.0f;
	zScale = 1.0f / (1.0f + position.z);
	viewBox.position = position.xy - Vec2(renderViewport.w, renderViewport.h) * (0.5f / zScale);
	viewBox.size = Vec2(renderViewport.w, renderViewport.h);

	if (game.GetInput().IsInputDown(SDLK_s)) {
		position.z += 0.02f;
	}

	if (game.GetInput().IsInputDown(SDLK_w)) {
		position.z -= 0.02f;
	}
}

void Camera::Render() {
	DebugStringBox debug(Game::Main, 0, 20, 100, 100);

	debug.DrawString(StaticString<80>::FromFormat("Camera pos: %f %f %f", position.x, position.y, position.z));
	return;
}

void Camera::RenderSprite(SDL_Texture* texture, const Vec3& position, const Vec2& size, float rotation, const Vec2& rotationOrigin, bool hFlip, bool vFlip) {
	float zScale = Math::clampmax(1.0f / (position.z - this->position.z), 10.0f);
	Vec2 origin = (position.xy - this->position.xy) * zScale + (viewBox.size / 2);
	SDL_Rect destRect = {origin.x, origin.y, size.x * zScale, size.y * zScale};
	SDL_Point sdlRotationOrigin = {rotationOrigin.x * zScale, rotationOrigin.y * zScale};

	SDL_RenderCopyEx(game.GetRenderer(), texture, nullptr, &destRect, rotation, &sdlRotationOrigin, 
		(SDL_RendererFlip)((hFlip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (vFlip ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE)));
}