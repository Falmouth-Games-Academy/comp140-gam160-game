#pragma once
#include "Math.h"

class Camera {
public:
	void Update();
	void Render();

	// Set and get position
	const Vec3& GetPosition() const;
	void SetPosition(const Vec3& position);
	
	// Set whether or not the camera follows the player
	void SetFollowingPlayer(bool isFollowingPlayer);

	/* Renders a sprite at a given position in the world
		@param position: origin position to render the sprite at
		@param size: Size of the sprite
		@param rotation: Rotation of the sprite, in clockwise degrees
		@param rotationOrigin: Origin of rotation of the sprite, in pixels
		@param hFlip: Whether the sprite will be flipped horizontally
		@param vFlip: Whether the sprite will be flipped vertically
	*/
	void RenderSprite(struct SDL_Texture* texture,
		const Vec3& position, const Vec2& size, 
		float rotation = 0.0f, const Vec2& rotationOrigin = Vec2(0.0f, 0.0f), 
		bool hFlip = false, bool vFlip = false);
	
	// Converts a pixel relative to a window to a world coordinate and returns the result
	Vec3 ScreenToWorld(const Vec3& screenPoint);

	const float scalePerZ = 1.0f;

private:
	Vec3 position;

	Rect2 viewBox;

	bool8 isFollowingPlayer;
};