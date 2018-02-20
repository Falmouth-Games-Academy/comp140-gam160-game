#pragma once
#include "Math.h"

class Camera {
public:
	void Update();
	void Render();

	// Set and get position
	const Vec3& GetPosition() const;
	void SetPosition(const Vec3& position);

	// Go to a position smoothly. Speed determines the number of seconds it'll take to get there
	void GoToPosition(const Vec3& position, float speed = 1.0f);

	// Gets the position the camera is moving towards
	const Vec3& GetTargetPosition() const;
	
	// Zoom
	void SetZoomIntoCentre(float targetZoom);

	// Set whether or not the camera follows the player
	void SetFollowingPlayer(bool isFollowingPlayer);

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
		@param sprite: The sprite to render (todo: const mutable?)
		@param position: The position to render the sprite at
		@param rotation: The rotation of the sprite, in clockwise degrees
		@param hFlip: Whether the sprite will be flipped horizontally
		@param vFlip: Whether the sprite will be flipped vertically
	*/
	void RenderSprite(const class Sprite& sprite, const Vec3& position, float rotation = 0.0f, bool hFlip = false, bool vFlip = false);
	
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
	// The positiono of the camera
	Vec3 position = {0.0f, 0.0f, 0.0f};

	// The position that the camera is aiming for
	Vec3 targetPosition = {0.0f, 0.0f, 0.0f};
	
	// The speed of the camera
	float32 speed = 1.0f;

	// The area, in world space, that the camera can see at a distance of 1.0f
	Rect2 viewBox;

	// Whether the camera is following the player character
	bool8 isFollowingPlayer;
};