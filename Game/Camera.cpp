#include "Camera.h"
#include "Game.h"

void Camera::Update() {
	// Get the viewport size
	SDL_Rect renderViewport;
	SDL_RenderGetViewport(game.GetRenderer(), &renderViewport);

	// Update the camera viewport size based on scaling etc
	viewBox.size = Vec2((float)renderViewport.w, (float)renderViewport.h);
	viewBox.position = position.xy - viewBox.size * 0.5f;

	// Move the camera gradually to the target point
	position += (targetPosition - position) * 10.0f * game.GetDeltaTime();
	
	// Follow the player
	if (isFollowingPlayer) {
		position = game.GetPlayer().GetPosition() - viewBox.size * Vec2(0.3f, 0.4f);
		position.z -= 1.5f;
	}
}

void Camera::Render() {
	DebugStringBox debug(RenderScreen::Main, 0, 20, 100, 100);

	debug.DrawString(StaticString<80>::FromFormat("Camera pos: %f %f %f", position.x, position.y, position.z));
}

const Vec3& Camera::GetPosition() const {
	return position;
}

void Camera::SetPosition(const Vec3& position) {
	this->targetPosition = position;
	this->position = position;
}

void Camera::GoToPosition(const Vec3& position, float speed) {
	this->targetPosition = position;
}

const Vec3& Camera::GetTargetPosition() const {
	return targetPosition;
}

void Camera::SetZoomIntoCentre(float targetZoom) {
	position.z = targetZoom;
}

void Camera::SetFollowingPlayer(bool isFollowingPlayer) {
	this->isFollowingPlayer = isFollowingPlayer;
}

void Camera::RenderSprite(SDL_Texture* texture, const Vec3& position, const Vec2& size, float rotation, const Vec2& rotationOrigin, bool hFlip, bool vFlip) {
	if (position.z <= this->position.z) {
		// Don't render things behind the camera!
		return;
	}

	float zScale = Math::clampmax(1.0f / (position.z - this->position.z), 10.0f);
	Vec2 origin = (position.xy - this->position.xy) * zScale + (viewBox.size / 2);
	SDL_Rect destRect = {origin.x, origin.y, size.x * zScale, size.y * zScale};
	SDL_Point sdlRotationOrigin = {rotationOrigin.x * zScale, rotationOrigin.y * zScale};

	SDL_RenderCopyEx(game.GetRenderer(), texture, nullptr, &destRect, rotation, &sdlRotationOrigin, 
		(SDL_RendererFlip)((hFlip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (vFlip ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE)));
}

void Camera::RenderSprite(const Sprite& sprite, const Vec3& position, float rotation, bool hFlip, bool vFlip) {
	if (position.z <= this->position.z) {
		// Don't render things behind the camera!
		return;
	}

	if (SDL_Texture* texture = sprite.GetSDLTexture()) {
		// Render the sprite texture with positioning, scaling, Z depth scaling, and rotation around an origin
		float zScale = Math::clampmax(1.0f / (position.z - this->position.z), 10.0f);
		Vec2 cameraOrigin = this->position.xy - (viewBox.size * 0.5f) / zScale;
		Vec2 origin = (position.xy - cameraOrigin - sprite.GetOrigin()) * zScale;
		SDL_Point sdlRotationOrigin = {sprite.GetOrigin().x * zScale, sprite.GetOrigin().y * zScale};
		SDL_Rect sdlDestRect = {(int)origin.x, (int)origin.y, (int)(sprite.GetDimensions().x * zScale), (int)(sprite.GetDimensions().y * zScale)};

		SDL_RenderCopyEx(game.GetRenderer(), sprite.GetSDLTexture(), nullptr, &sdlDestRect, rotation, &sdlRotationOrigin, 
			(SDL_RendererFlip)((hFlip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (vFlip ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE)));
	}
}
	
void Camera::RenderRectangle(const Vec3& position, const Vec2& size, Colour colour) {
	float zScale = Math::clampmax(1.0f / (position.z - this->position.z), 10.0f);

	Vec2 origin = WorldToScreen(position).xy;
	SDL_Rect sdlDestRect = {(int)origin.x, (int)origin.y, (int)(size.x * zScale), (int)(size.y * zScale)}; // todo: consider rounding up so things don't go invisible

	SDL_SetRenderDrawColor(game.GetRenderer(), colour.r, colour.g, colour.b, colour.a);
	SDL_RenderDrawRect(game.GetRenderer(), &sdlDestRect);
}

Vec3 Camera::ScreenToWorld(const Vec3& screenPoint) const {
	float zScale = Math::clampmax(1.0f / (screenPoint.z - this->position.z), 10.0f);
	Vec2 cameraOrigin = position.xy - (viewBox.size * 0.5f) / zScale;

	return Vec3(cameraOrigin + screenPoint.xy / zScale, screenPoint.z);
}

Vec3 Camera::WorldToScreen(const Vec3& worldPoint) const {
	float zScale = Math::clampmax(1.0f / (worldPoint.z - this->position.z), 10.0f);
	Vec2 cameraOrigin = position.xy - (viewBox.size * 0.5f) / zScale;

	return Vec3((worldPoint.xy - cameraOrigin) * zScale, 1.0f);
	// world = view + (screen / scale)
	// scale = view - 
}
