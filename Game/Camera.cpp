#include "Camera.h"
#include "Game.h"

void Camera::Update(float deltaTime) {
	// Get the viewport size
	SDL_Rect renderViewport;
	SDL_RenderGetViewport(game.GetRenderer(), &renderViewport);

	// Update the camera viewport size based on scaling etc
	viewBox.size = Vec2((float)renderViewport.w, (float)renderViewport.h);
	viewBox.position = position.xy - viewBox.size * 0.5f;

	// Move the camera gradually to the target point
	position += (targetPosition - position) * 10.0f * deltaTime;
	
	// Follow the player
	if (isFollowingPlayer) {
		position = game.GetPlayer().GetPosition() - viewBox.size * Vec2(0.3f, 0.4f);
		position.z -= 1.5f;
	}

	// Apply shake effect
	if (shakeRate >= 0.002f /* no division by 0 */ && shakeTimer > 0.0f) {
		Vec3 lastShakeOffset = shakeOffset;
		float shakeMagnitude = shakeInitialMagnitude * shakeTimer / shakeInitialTimer;
		
		// Determine the current shake offest
		shakeOffset.y = sin((shakeInitialTimer - shakeTimer) * Math::pi * 2.0f * shakeRate) * shakeMagnitude;

		// Update the final position
		position = position - lastShakeOffset + shakeOffset;

		// Count down the shake timer
		shakeTimer -= deltaTime;
	} else {
		shakeOffset = Vec3(0.0f, 0.0f, 0.0f);
	}
}

void Camera::Render() {
	// Render debug information
	if (DebugStringBox* debug = game.GetDebug()) {
		debug->DrawString(StaticString<80>::FromFormat("Camera pos: %f %f %f", position.x, position.y, position.z));
	}
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

void Camera::StartShake(float32 time, float32 rate, float32 magnitude) {
	shakeTimer = shakeInitialTimer = time;
	shakeRate = rate;
	shakeInitialMagnitude = magnitude;
}

void Camera::RenderSprite(SDL_Texture* texture, const Vec3& position, const Vec2& size, float rotation, const Vec2& rotationOrigin, bool hFlip, bool vFlip) {
	if (position.z <= this->position.z) {
		// Don't render things behind the camera!
		return;
	}

	float zScale = Math::clampmax(1.0f / (position.z - this->position.z), 10.0f);
	Vec2 origin = (position.xy - this->position.xy) * zScale + (viewBox.size / 2);
	SDL_Rect destRect = {(int)origin.x, (int)origin.y, (int)(size.x * zScale), (int)(size.y * zScale)};
	SDL_Point sdlRotationOrigin = {(int)(rotationOrigin.x * zScale), (int)(rotationOrigin.y * zScale)};

	SDL_RenderCopyEx(game.GetRenderer(), texture, nullptr, &destRect, rotation, &sdlRotationOrigin, 
		(SDL_RendererFlip)((hFlip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (vFlip ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE)));
}

void Camera::RenderSprite(const Sprite& sprite, const Vec3& position, float rotation, uint32 flipFlags, const Rect2* region) {
	// Simply render the sprite's current frame
	if (sprite.GetCurrentFrame() != nullptr) {
		RenderSpriteFrame(*sprite.GetCurrentFrame(), position, rotation, flipFlags, region);
	}
}

void Camera::RenderSpriteFrame(const Sprite& sprite, int frameIndex, const Vec3& position, float rotation, uint32 flipFlags, const Rect2* region) {
	if (sprite.GetFrame(frameIndex)) {
		RenderSpriteFrame(*sprite.GetFrame(frameIndex), position, rotation, flipFlags, region);
	}
}

void Camera::RenderSpriteFrame(const SpriteFrame& sprite, const Vec3& position, float rotation, uint32 flipFlags, const Rect2* region) {
	if (position.z <= this->position.z) {
		// Don't render things behind the camera!
		return;
	}

	if (SDL_Texture* texture = sprite.GetSDLTexture()) {
		// Render the sprite texture with positioning, scaling, Z depth scaling, and rotation around an origin
		float zScale = Math::clampmax(1.0f / (position.z - this->position.z), 10.0f);
		Vec2 cameraOrigin = this->position.xy - (viewBox.size * 0.5f) / zScale;
		Vec2 origin = (position.xy - cameraOrigin - sprite.GetOrigin()) * zScale;
		SDL_Point sdlRotationOrigin = {(int)(sprite.GetOrigin().x * zScale), (int)(sprite.GetOrigin().y * zScale)};
		SDL_Rect sdlDestRect = {(int)origin.x, (int)origin.y, (int)(sprite.GetDimensions().x * zScale), (int)(sprite.GetDimensions().y * zScale)};

		// Setup flip flags
		SDL_RendererFlip sdlFlip = SDL_FLIP_NONE;

		if (flipFlags == FlipFlags::Horizontal) {
			sdlFlip = SDL_FLIP_HORIZONTAL;
		} else if (flipFlags == FlipFlags::Vertical) {
			sdlFlip = SDL_FLIP_VERTICAL;
		} else if (flipFlags == (FlipFlags::Vertical | FlipFlags::Horizontal)) {
			rotation += 180.0f; // This little detail might be why SDL doesn't have SDL_FLIP_BOTH
		}

		// If a sprite region was specified, draw the sprite with wrapping enabled
		if (region) {
			SDL_Rect sdlSourceRect = {(int)region->x, (int)region->y, (int)region->width, (int)region->height};

			sdlSourceRect.x = region->x;
			sdlSourceRect.y = region->y;
			sdlSourceRect.w = region->width;
			sdlSourceRect.h = region->height;
			sdlDestRect.w -= sdlSourceRect.x;
			sdlDestRect.h -= sdlSourceRect.y;
			// Todo make this actually work

			SDL_RenderCopyEx(game.GetRenderer(), sprite.GetSDLTexture(), &sdlSourceRect, &sdlDestRect, rotation, &sdlRotationOrigin, sdlFlip);
		} else {
			// Otherwise draw it normally
			SDL_RenderCopyEx(game.GetRenderer(), sprite.GetSDLTexture(), nullptr, &sdlDestRect, rotation, &sdlRotationOrigin, sdlFlip);
		}
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
