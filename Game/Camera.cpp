#include "Camera.h"
#include "Game.h"

void Camera::Update(float deltaTime) {
	// Get the viewport size
	SDL_Rect renderViewport;
	SDL_RenderGetViewport(game.GetSDLRenderer(), &renderViewport);

	// Update the camera viewport size based on scaling etc
	viewBox.size = Vec2((float)renderViewport.w, (float)renderViewport.h);
	viewBox.position = position.xy - viewBox.size * 0.5f;

	// Move the camera gradually to the target point
	position += (targetPosition - position) * 10.0f * deltaTime;
	
	// Follow the player
	if (isFollowingPlayer) {
		playerFollowOffset.xy = viewBox.size * Vec2(-0.3f, -0.4f);

		position = game.GetPlayer().GetPosition() + playerFollowOffset;
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

	// Zoom in/out
	if (game.GetInput().IsKeyDown(SDLK_c) && game.GetInput().GetMouseScroll()) {
		playerFollowOffset.z += game.GetInput().GetMouseScroll() * 0.5f;
	}
}

void Camera::Render() {
	// Render layers
	FlushLayeredRenders();
}

void Camera::FlushLayeredRenders() {
	// Perform every deferred render call in descending order
	int lastMajorItem;
	float32 lastMajorDepth = 10000.0f; // Hack
	const int numCalls = renderCalls.GetNum();

	for (int iteration = 0; iteration < numCalls; ++iteration) {
		float32 lastMinorDepth = -10000.0f; // Hack
		int lastMinorItem = -1;

		for (int item = 0; item < numCalls; ++item) {
			// Select this item if it's smaller depth than the last rendered item
			// Or if the depth is the same as the last item, render it if it has a higher index than the last
			float depth = renderCalls[item]->depth;
			if ((renderCalls[item]->depth < lastMajorDepth && renderCalls[item]->depth > lastMinorDepth) || 
				(renderCalls[item]->depth == lastMajorDepth && item > lastMajorItem)) {
				lastMinorItem = item;
				lastMinorDepth = renderCalls[item]->depth;

				if (renderCalls[item]->depth == lastMajorDepth) {
					break;
				}
			}
		}

		// If an item was found (this should always be true, but if the above hack stays, then it isn't necessarily)
		if (lastMinorItem != -1) {
			RenderCall* call = renderCalls[lastMinorItem];

			if (!call->texture) {
				continue;
			}

			// Render the sprite texture with positioning, scaling, Z depth scaling, and rotation around an origin
			SDL_Point sdlRotationOrigin = {(int)call->rotationOrigin.x, (int)call->rotationOrigin.y};
			SDL_Rect sdlDestRect = {(int)call->destRect.x, (int)call->destRect.y, (int)call->destRect.width, (int)call->destRect.height};

			// Setup flip flags
			SDL_RendererFlip sdlFlip = SDL_FLIP_NONE;

			if (call->flipFlags == FlipFlags::Horizontal) {
				sdlFlip = SDL_FLIP_HORIZONTAL;
			} else if (call->flipFlags == FlipFlags::Vertical) {
				sdlFlip = SDL_FLIP_VERTICAL;
			} else if (call->flipFlags == (FlipFlags::Vertical | FlipFlags::Horizontal)) {
				call->rotation += 180.0f; // This little detail might be why SDL doesn't have SDL_FLIP_BOTH
			}

			// Setup render colour blend
			SDL_SetTextureColorMod(const_cast<SDL_Texture*>(call->texture), call->colourBlend.r, call->colourBlend.g, call->colourBlend.b);

			// If a sprite region was specified, draw the sprite with wrapping enabled
			if (!call->useFullRegion) {
				SDL_Rect sdlSourceRect = {(int)call->region.x, (int)call->region.y, (int)call->region.width, (int)call->region.height};

				sdlDestRect.w -= sdlSourceRect.x;
				sdlDestRect.h -= sdlSourceRect.y;
				// Todo make this actually work

				SDL_RenderCopyEx(game.GetSDLRenderer(), const_cast<SDL_Texture*>(call->texture), &sdlSourceRect, &sdlDestRect, call->rotation, &sdlRotationOrigin, sdlFlip);
			} else {
				// Otherwise draw it normally
				SDL_RenderCopyEx(game.GetSDLRenderer(), const_cast<SDL_Texture*>(call->texture), nullptr, &sdlDestRect, call->rotation, &sdlRotationOrigin, sdlFlip);
			}

			// Update major item
			lastMajorDepth = lastMinorDepth;
			lastMajorItem = lastMinorItem;
		}
	}

	// Delete them all
	for (RenderCall* call : renderCalls) {
		delete call;
	}

	renderCalls.Clear();
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

const Vec2& Camera::GetViewSize() const {
	return viewBox.size;
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

void Camera::RenderSprite(const Sprite& sprite, const Vec3& position, float rotation, uint32 flipFlags, const Rect2* region) {
	// Simply render the sprite's current frame
	if (sprite.GetCurrentFrame() != nullptr) {
		RenderSpriteFrame(*sprite.GetCurrentFrame(), position, rotation, flipFlags, region, sprite.GetBlendColour());
	}
}

void Camera::RenderSpriteFrame(const Sprite& sprite, int frameIndex, const Vec3& position, float rotation, uint32 flipFlags, const Rect2* region, const Colour& blendColour) {
	if (sprite.GetFrame(frameIndex)) {
		RenderSpriteFrame(*sprite.GetFrame(frameIndex), position, rotation, flipFlags, region, blendColour);
	}
}

void Camera::RenderSpriteFrame(const SpriteFrame& sprite, const Vec3& position, float rotation, uint32 flipFlags, const Rect2* region, const Colour& blendColour) {
	if (position.z <= this->position.z) {
		// Don't render things behind the camera!
		return;
	}

	// Render the sprite texture with positioning, scaling, Z depth scaling, and rotation around an origin
	if (SDL_Texture* texture = sprite.GetSDLTexture()) {
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

			sdlDestRect.w -= sdlSourceRect.x;
			sdlDestRect.h -= sdlSourceRect.y;
			// Todo make this actually work

			InsertNewRenderCall(new RenderCall(position.z - this->position.z, texture, region, Rect2(origin, sprite.GetDimensions() * zScale), rotation, sprite.GetOrigin() * zScale, sdlFlip, blendColour));
		} else {
			// Otherwise draw it normally
			InsertNewRenderCall(new RenderCall(position.z - this->position.z, texture, nullptr, Rect2(origin, sprite.GetDimensions() * zScale), rotation, sprite.GetOrigin() * zScale, sdlFlip, blendColour));
		}
	}
}

void Camera::RenderRectangle(const Vec3& position, const Vec2& size, Colour colour) {
	float zScale = Math::clampmax(1.0f / (position.z - this->position.z), 10.0f);

	Vec2 origin = WorldToScreen(position).xy;
	SDL_Rect sdlDestRect = {(int)origin.x, (int)origin.y, (int)(size.x * zScale), (int)(size.y * zScale)}; // todo: consider rounding up so things don't go invisible

	SDL_SetRenderDrawColor(game.GetSDLRenderer(), colour.r, colour.g, colour.b, colour.a);
	SDL_RenderDrawRect(game.GetSDLRenderer(), &sdlDestRect);
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

void Camera::InsertNewRenderCall(RenderCall* renderCall) {
	// Insert into the render call list (unordered for now)
	renderCalls.Append(renderCall);
}
