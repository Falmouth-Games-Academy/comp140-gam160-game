#pragma once
#include "Math.h"
#include "Containers.h"
#include "String.h"

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

	// Renders depth-layered assets
	void FlushLayeredRenders();

	// Set and get position
	const Vec3& GetPosition() const;
	void SetPosition(const Vec3& position);

	// Go to a position smoothly. Speed determines the number of seconds it'll take to get there
	void GoToPosition(const Vec3& position, float speed = 1.0f);

	// Gets the position the camera is moving towards
	const Vec3& GetTargetPosition() const;

	// Gets the size of the viewport at a distance of 1.0f
	const Vec2& GetViewSize() const;
	
	// Gets the size of the viewport at an arbitrary Z value
	const Vec2 GetViewSize(float z) const;

public:
	// Sets the camera's current zoom, homing in on the centre of view
	void SetZoomIntoCentre(float targetZoom);

	// Set whether or not the camera follows the player
	void SetFollowingPlayer(bool isFollowingPlayer);

	// Adds a view target. This widens the view so you can see additional objects in the scene if they're close enough. Reset every frame.
	void AddViewTarget(const Vec3& targetPosition, const Vec2& targetSize);

	/* Starts a camera shake (cancelling out any previous shake operation)
		@param time: Time, in seconds, that the shake will last
		@param rate: Rate, in Hz or shakes per second, that the shake will occur at
		@param magnitude: Distance that the shake will send the camera up and down, in positional units
	*/
	void StartShake(float32 time, float32 rate, float32 magnitude);

public:
	/* Note: Camera render functions work in world space! This depends entirely on the location of the camera, including its Z position */
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
		@param blendColour: An optional colour to modulate the sprite by during render
	*/
	void RenderSpriteFrame(const class SpriteFrame& sprite, const Vec3& position, float rotation = 0.0f, uint32 flipFlags = 0, const Rect2* region = nullptr, const Colour& blendColour = Colour::White());

	/* Renders a single sprite frame at a given position in the world
		@param sprite: The sprite to render
		@param frameIndex: The index of the frame to render
		@param position: The position to render the sprite at
		@param rotation: The rotation of the sprite, in clockwise degrees
		@param flipFlags: Which axes to flip the image, if any (of FlipFlags)
		@param region: The region of the sprite to draw, in pixel coordinates of the original unscaled image 
		@param blendColour: An optional colour to modulate the sprite by during render
	*/
	void RenderSpriteFrame(const class Sprite& sprite, int frameIndex, const Vec3& position, float rotation = 0.0f, uint32 flipFlags = 0, const Rect2* region = nullptr, const Colour& blendColour = Colour::White());

	/* Renders a rectangle at a given position in the world 
		@param position: Position that the rectangle should be drawn
		@param size: Dimensions of the rectangle
		@param colour: The colour of the rectangle
		@param blendColour: An optional colour to modulate the sprite by during render
	*/
	void RenderRectangle(const Vec3& position, const Vec2& size, Colour colour);

	/* Renders a text at a given position in the world 
		@param string: Text to render
		@param position: Position of the text to render
		@param colour: Colour of the text
		@param isCentered: Whether the text is centered on the position given 
		@param isLarge: Whether the text uses the large font 
	*/
	void RenderText(const char* string, const Vec3& position, const Colour& colour = Colour::Black(), bool isCentered = false, bool isLarge = false);

public:
	// Converts a pixel relative to a window to a world coordinate and returns the result
	Vec3 ScreenToWorld(const Vec3& screenPoint) const;

	// Converts a pixel from a world coordinate to a window coordinate and returns the result
	Vec3 WorldToScreen(const Vec3& worldPoint) const;

private:
	/* Inserts a render call into the ordered render call list.
	   Delete will be called on the render call when it's no longer needed! */
	void InsertNewRenderCall(struct RenderCall* renderCall);

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
	Vec3 playerFollowOffset = (Vec3(0.0f, 0.0f, -3.0f));

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

	// List of depth-ordered render calls
	Array<struct RenderCall*> renderCalls;

private:
	// Constants
	const float scalePerZ = 1.0f;

	// Min and maximum zooms
	const float minZ = -7.0f;
	const float maxZ = 10.0f;
};

struct RenderCall {
	// Type of render call
	enum Type : uint8 {
		Sprite,
		Text
	};

	// Sprite draw constructor
	RenderCall(float32 depth_, const struct SDL_Texture* texture_, const Rect2* srcRect_, const Rect2& destRect_, const float rotation_, const Vec2& rotationOrigin_, uint32 flipFlags_, 
		const Colour& colourBlend_) : 
			depth(depth_), texture(texture_), destRect(destRect_), rotation(rotation_), flipFlags(flipFlags_), rotationOrigin(rotationOrigin_), 
			colourBlend(colourBlend_), type(Sprite) {
		if (srcRect_) {
			srcRect = *srcRect_;
			useFullRegion = false;
		} else {
			useFullRegion = true;
		}
	}
	
	// Text draw constructor
	RenderCall(const Vec3& position, const char* text, bool isCentered, bool isLarge) : textPosition(position), textContents(text), textIsLarge(isLarge), textIsCentered(isCentered), 
			type(Text) {
		depth = position.z;
	}

	// Text draw constructor

	// Parameters for the draw call
	Type type;

	const struct SDL_Texture* texture;
	Rect2 srcRect, destRect;

	float32 depth;

	float32 rotation;
	Vec2 rotationOrigin;

	uint32 flipFlags;

	Colour colourBlend;

	Rect2 region;
	bool8 useFullRegion;

	StaticString<256> textContents;

	Vec3 textPosition;

	bool8 textIsLarge;
	bool8 textIsCentered;
};
