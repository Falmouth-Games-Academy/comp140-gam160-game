#pragma once
#include "Render.h"
#include "Image.h"
#include "Containers.h" // Array

// A sprite frame is an Image with an origin, scale and/or any other future effects to come. It also keeps a texture cache.
class SpriteFrame {
public:
	/* Constructors and destructors */
	SpriteFrame() : baseOrigin(0.0f, 0.0f), scaledOrigin(0.0f, 0.0f), scaledDimensions(0.0f, 0.0f), scale(0.0f, 0.0f), textures{nullptr}, 
		sourceImage(nullptr) {};

	// Constructor: Tries to load the image and store it
	inline SpriteFrame(const char* filename, const Vec2& origin_ = Vec2(0.0f, 0.0f), const Vec2& scale_ = Vec2(1.0f, 1.0f));

	// Copy constructor
	inline SpriteFrame(const SpriteFrame& sprite) : baseOrigin(sprite.baseOrigin), scaledOrigin(sprite.scaledOrigin), scaledDimensions(sprite.scaledDimensions), 
		scale(sprite.scale), textures{nullptr}, sourceImage(sprite.sourceImage) {};

	// Destructor: Frees image etc
	~SpriteFrame();

	// Frees the texture cache
	void ClearTextureCache();

public:
	/* Loading */
	// Discards any previous data and loads a new sprite
	void Load(const char* filename, const Vec2& origin = Vec2(0.0f, 0.0f), const Vec2& scale = Vec2(1.0f, 1.0f));

public:
	/* Getters and setters */
	bool IsLoaded() const;

	// Returns the origin of the sprite, in pixels, relative to the image as it is currently scaled
	inline const Vec2& GetOrigin() const;

	// Returns the origin of the sprite, in pixels, relative to the original unscaled image
	inline const Vec2& GetBaseOrigin() const;

	// Sets the origin, in pixels relative to the original image
	inline void SetBaseOrigin(const Vec2& origin);

	// Gets the dimensions of the image, in pixels, as it is currently scaled
	inline const Vec2& GetDimensions() const;

	// Gets the dimensions of the image, in pixels, according to the original unscaled image
	inline const Vec2 GetBaseDimensions() const;

	// Sets the scale of the image, affecting its dimensions
	inline void SetScale(const Vec2& scale);

	// Gets the scale of the image
	inline const Vec2& GetScale() const;

	// Returns a texture of the image, generating one if it doesn't exist
	// Todo: true const correctness?
	struct SDL_Texture* GetSDLTexture(RenderScreen screen = RenderScreen::Main) const;

public:
	// Tools
	// PixelToWorld: Converts a pixel coordinate on this sprite to a world coordinate
	inline Vec3 PixelToWorld(const Vec2& spritePoint, const Vec3& worldPoint, float rotation) const;

private:
	// Pointer to the source image
	const Image* sourceImage = nullptr;

	// Origin of the sprite, in pixels relative to the original unscaled image
	Vec2 baseOrigin;

	// Origin of the sprite, in pixels relative to the scaled image
	Vec2 scaledOrigin;

	// Dimensions of the sprite, in pixels relative to the scaled image
	Vec2 scaledDimensions; // todo: float Dimensions2 type?

	// Scale factor of the sprite
	Vec2 scale;

	// Texture cache
	mutable SDL_Texture* textures[RenderScreen::NumRenderScreens];
};

// A sprite is a collection of sprite frames which can be animated and used as a game object
class Sprite {
public:
	/* Construction and destruction */
	inline Sprite() : currentFrame(0) {};

	// Load a single-frame sprite with an origin and scale
	inline Sprite(const char* filename, const Vec2& origin_ = Vec2(0.0f, 0.0f), const Vec2& scale_ = Vec2(1.0f, 1.0f));

	// Copies a sprite
	inline Sprite(const Sprite& other);

	// Unloads a sprite and frees frames from memory
	inline ~Sprite();

public:
	/* Loading and destroying frames */
	// Loads a single frame into the sprite, adding to any existing frames, and returns whether successful
	inline bool LoadFrame(const char* filename, const Vec2& origin = Vec2(0.0f, 0.0f), const Vec2& scale = Vec2(1.0f, 1.0f));

	// Loads several frames into the sprite in the format filename_%i.png, where %i is the current frame index. Returns whether all frames were loaded.
	bool LoadFrames(const char* filename, int numFrames, const Vec2& origin = Vec2(0.0f, 0.0f), const Vec2& scale = Vec2(1.0f, 1.0f));

	// Removes a frame at the given index
	void RemoveFrame(int frameIndex);

	// Clears all frames, freeing them
	inline void ClearFrames();

public:
	/* Image information getters and setters */

	// Returns the scaled dimensions of a frame, if the frame exists
	inline Vec2 GetDimensions(int frameIndex = 0) const;

	// Returns the original dimensions of a frame, if the frame exists
	inline Vec2 GetBaseDimensions(int frameIndex = 0) const;

public:
	/* Retrieving frames */
	// Gets a frame, if it exists
	inline SpriteFrame* GetFrame(int index);
	inline const SpriteFrame* GetFrame(int index) const;

	// Gets all frames
	inline Array<SpriteFrame*>& GetFrames();
	inline const Array<SpriteFrame*>& GetFrames() const;

	// Sets the current frame
	inline void SetCurrentFrame(float32 index);

	// Gets the current frame, if it exists
	inline const SpriteFrame* GetCurrentFrame() const;

	// Returns the number of frames
	inline int GetNumFrames() const;

public:
	/* Animation */
	// Performs a standard frame progression based on the animation speed
	void TickAnimation(float deltaTime);

	// Sets the animation speed in frames/sec
	void SetFrameRate(float32 fps);

public:
	/* Scaling and other effects */
	// Sets the scale of all the frames in the sprites
	void SetScale(const Vec2& scale);

	// Gets the scale of the current frame
	const Vec2 GetScale() const;

	// Gets the origin of the current frame
	const Vec2 GetOrigin() const;

	// Todo standardise which frames are being affected/used lol

private:
	// Array of all frames in the sprite
	Array<SpriteFrame*> frames;

	// Index of the currently active frame
	float32 currentFrame = 0;

	// Animation speed in frames/sec
	float32 frameRate = 10.0f;
};

inline Sprite::Sprite(const char* filename, const Vec2& origin, const Vec2& scale) {
	LoadFrame(filename, origin, scale);
}

inline Sprite::Sprite(const Sprite& other) {
	// Copy sprite frames
	for (SpriteFrame* frame : other.frames) {
		frames.Append() = new SpriteFrame(*frame);
	}

	currentFrame = other.currentFrame;
	frameRate = other.frameRate;
}

inline Sprite::~Sprite() {
	// Cleanup frames
	ClearFrames();
}

inline bool Sprite::LoadFrame(const char* filename, const Vec2& origin, const Vec2& scale) {
	// Append a shiny new frame
	frames.Append() = new SpriteFrame(filename, origin, scale);

	// Make sure it loaded and clean it up if it didn't
	if (frames[0]->IsLoaded()) {
		// Success! Yay!
		return true;
	} else {
		// Failure! Yay!
		RemoveFrame(frames.GetNum() - 1);
		return false;
	}
}

inline void Sprite::ClearFrames() {
	// Destroy all loaded frames
	for (SpriteFrame* frame : frames) {
		delete frame;
	}

	frames.Clear();
}

inline Vec2 Sprite::GetDimensions(int frameIndex) const {
	if (frames.IsIndexValid(frameIndex)) {
		return frames[frameIndex]->GetDimensions();
	} else {
		return Vec2(0.0f, 0.0f);
	}
}

inline Vec2 Sprite::GetBaseDimensions(int frameIndex) const {
	if (frames.IsIndexValid(frameIndex)) {
		return frames[frameIndex]->GetBaseDimensions();
	} else {
		return Vec2(0.0f, 0.0f);
	}
}

inline SpriteFrame* Sprite::GetFrame(int index) {
	if (frames.IsIndexValid(index)) {
		return frames[index];
	} else {
		return nullptr;
	}
}

inline const SpriteFrame* Sprite::GetFrame(int index) const {
	if (frames.IsIndexValid(index)) {
		return frames[index];
	} else {
		return nullptr;
	}
}

inline Array<SpriteFrame*>& Sprite::GetFrames() {
	return frames;
}

inline const Array<SpriteFrame*>& Sprite::GetFrames() const {
	return frames;
}

inline void Sprite::SetCurrentFrame(float32 index) {
	currentFrame = index;
}

inline const Vec2 Sprite::GetScale() const {
	if (const SpriteFrame* frame = GetCurrentFrame()) {
		return frame->GetDimensions();
	} else {
		return Vec2(0.0f, 0.0f);
	}
}

inline const Vec2 Sprite::GetOrigin() const {
	if (const SpriteFrame* frame = GetCurrentFrame()) {
		return frame->GetOrigin();
	} else {
		return Vec2(0.0f, 0.0f);
	}
}

inline bool SpriteFrame::IsLoaded() const {
	return sourceImage != nullptr;
}

inline const SpriteFrame* Sprite::GetCurrentFrame() const {
	return GetFrame((int)currentFrame);
}

inline int Sprite::GetNumFrames() const {
	return frames.GetNum();
}

inline void Sprite::SetFrameRate(float32 fps) {
	frameRate = fps;
}

inline SpriteFrame::SpriteFrame(const char* filename, const Vec2& origin_, const Vec2& scale_) : textures{nullptr} {
	Load(filename, origin_, scale_);
}

inline const Vec2& SpriteFrame::GetBaseOrigin() const {
	return baseOrigin;
}

inline const Vec2& SpriteFrame::GetOrigin() const {
	return scaledOrigin;
}

inline void SpriteFrame::SetBaseOrigin(const Vec2& origin) {
	baseOrigin = origin;
	scaledOrigin = origin * scale;
}

inline const Vec2& SpriteFrame::GetDimensions() const {
	return scaledDimensions;
}

inline const Vec2 SpriteFrame::GetBaseDimensions() const {
	if (sourceImage) {
		return Vec2((float)sourceImage->GetDimensions().width, (float)sourceImage->GetDimensions().height);
	} else {
		return Vec2(0.0f, 0.0f);
	}
}

inline void SpriteFrame::SetScale(const Vec2& scale) {
	this->scale = scale;
	scaledOrigin = baseOrigin * scale;

	if (sourceImage) {
		scaledDimensions = Vec2((float)sourceImage->GetDimensions().width, (float)sourceImage->GetDimensions().height) * scale;
	} else {
		scaledDimensions = Vec2(0.0f, 0.0f);
	}
}

inline const Vec2& SpriteFrame::GetScale() const {
	return scale;
}

inline Vec3 SpriteFrame::PixelToWorld(const Vec2& pixelPosition, const Vec3& worldPosition, float rotation) const {
	return worldPosition + ((pixelPosition - baseOrigin) * scale).Rotated(rotation);
}