#pragma once
#include "Math.h"

namespace FlipFlags {
	const uint32
		None = 0,
		Horizontal = 1,
		Vertical = 2;
};

class Camera {
public:
	void Update(float deltaTime);
	void Render();

	// Set and get position
	const Vec3& GetPosition() const;
	void SetPosition(const Vec3& position);

	// Go to a position smoothly. Speed determines the number of seconds it'll take to get there
	void GoToPosition(const Vec3& position, float speed = 1.0f);

	// Gets the position the camera is moving towards
	const Vec3& GetTargetPosition() const;
	
	// Sets the camera's current zoom, homing in on the centre of view
	void SetZoomIntoCentre(float targetZoom);

	// Set whether or not the camera follows the player
	void SetFollowingPlayer(bool isFollowingPlayer);

	/* Starts a camera shake (cancelling out any previous shake operation)
		@param time: Time, in seconds, that the shake will last
		@param rate: Rate, in Hz or shakes per second, that the shake will occur at
		@param magnitude: Distance that the shake will send the camera up and down, in positional units
	*/
	void StartShake(float32 time, float32 rate, float32 magnitude);

	/* Note: Camera render functions work in world space! This depends entirely on the location of the camera, including its Z position */
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

	/* Renders a sprite at a given position in the world
		@param sprite: The sprite to render
		@param position: The position to render the sprite at
		@param rotation: The rotation of the sprite, in clockwise degrees
		@param flipFlags: Which axes to flip the image, if any (of FlipFlags)
		@param region: The region of the sprite to draw, in pixel coordinates of the original unscaled image 
	*/
	void RenderSprite(const class Sprite& sprite, const Vec3& position, float rotation = 0.0f, uint32 flipFlags = 0, const Rect2* region = nullptr);

	/* Renders a single sprite frame at a given position in the world
		@param sprite: The sprite frame to render
		@param position: The position to render the sprite at
		@param rotation: The rotation of the sprite, in clockwise degrees
		@param flipFlags: Which axes to flip the image, if any (of FlipFlags)
		@param region: The region of the sprite to draw, in pixel coordinates of the original unscaled image 
	*/
	void RenderSpriteFrame(const class SpriteFrame& sprite, const Vec3& position, float rotation = 0.0f, uint32 flipFlags = 0, const Rect2* region = nullptr);

	/* Renders a single sprite frame at a given position in the world
		@param sprite: The sprite to render
		@param frameIndex: The index of the frame to render
		@param position: The position to render the sprite at
		@param rotation: The rotation of the sprite, in clockwise degrees
		@param flipFlags: Which axes to flip the image, if any (of FlipFlags)
		@param region: The region of the sprite to draw, in pixel coordinates of the original unscaled image 
	*/
	void RenderSpriteFrame(const class Sprite& sprite, int frameIndex, const Vec3& position, float rotation = 0.0f, uint32 flipFlags = 0, const Rect2* region = nullptr);

	/* Renders a rectangle at a given position in the world 
		@param position: Position that the rectangle should be drawn
		@param size: Dimensions of the rectangle
		@param colour: The colour of the rectangle
	*/
	void RenderRectangle(const Vec3& position, const Vec2& size, Colour colour);

	// Converts a pixel relative to a window to a world coordinate and returns the result
	Vec3 ScreenToWorld(const Vec3& screenPoint) const;

	// Converts a pixel from a world coordinate to a window coordinate and returns the result
	Vec3 WorldToScreen(const Vec3& worldPoint) const;

	const float scalePerZ = 1.0f;

private:
	// The position of the camera
	Vec3 position = {0.0f, 0.0f, 0.0f};

	// The position that the camera is aiming for
	Vec3 targetPosition = {0.0f, 0.0f, 0.0f};
	
	// The speed of the camera
	float32 speed = 1.0f;

	// The area, in world space, that the camera can see at a distance of 1.0f
	Rect2 viewBox;

	// Whether the camera is following the player character
	bool8 isFollowingPlayer;

	// The offset of the camera relative to the player, when following
	Vec3 playerFollowOffset = (Vec3(0.0f, 0.0f, -1.5f));

	// Shake effect stuff
	// Current time, in secs, until the shake is finished
	float32 shakeTimer = 0.0f;
	float32 shakeInitialTimer = 0.0f;

	// Rate of shake, in Hz
	float32 shakeRate = 0.0f;

	// Initial magnitude of the shake
	float32 shakeInitialMagnitude = 0.0f;

	// Current positional offset due to the shake
	Vec3 shakeOffset = {0.0f, 0.0f, 0.0f};
};