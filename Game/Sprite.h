#pragma once
#include "Render.h"
#include "Image.h"

// A sprite is essentially an image for quick reuse for game objects.
// It has an origin, scale and/or any other future effects to come. It also keeps a texture cache.
class Sprite : public Image {
public:
	Sprite() : originalOrigin(0.0f, 0.0f), scaledOrigin(0.0f, 0.0f), scaledDimensions(0.0f, 0.0f), scale(0.0f, 0.0f), textures{nullptr} {};

	// Constructor: Tries to load the image and store it
	inline Sprite(const char* fileName, const Vec2& origin_ = Vec2(0.0f, 0.0f), const Vec2& scale_ = Vec2(1.0f, 1.0f));

	// Copy constructor
	inline Sprite(const Sprite& sprite) : Image(sprite), originalOrigin(sprite.originalOrigin), scaledOrigin(sprite.scaledOrigin), scaledDimensions(sprite.scaledDimensions), 
		scale(sprite.scale), textures{nullptr} {};

	// Destructor: Frees image etc
	~Sprite();

	// Discards any previous data and loads a new sprite
	void Load(const char* filename, const Vec2& origin = Vec2(0.0f, 0.0f), const Vec2& scale = Vec2(1.0f, 1.0f));

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

private:
	// Origin of the sprite, in pixels relative to the original unscaled image
	Vec2 originalOrigin;

	// Origin of the sprite, in pixels relative to the scaled image
	Vec2 scaledOrigin;

	// Dimensions of the sprite, in pixels relative to the scaled image
	Vec2 scaledDimensions; // todo: float Dimensions2 type?

	// Scale factor of the sprite
	Vec2 scale;

	// Texture cache
	mutable SDL_Texture* textures[RenderScreen::NumRenderScreens];
};

inline Sprite::Sprite(const char* fileName, const Vec2& origin_, const Vec2& scale_) : textures{nullptr} {
	Load(fileName, origin_, scale_);
}

inline const Vec2& Sprite::GetBaseOrigin() const {
	return originalOrigin;
}

inline const Vec2& Sprite::GetOrigin() const {
	return scaledOrigin;
}

inline void Sprite::SetBaseOrigin(const Vec2& origin) {
	originalOrigin = origin;
	scaledOrigin = origin * scale;
}

inline const Vec2& Sprite::GetDimensions() const {
	return scaledDimensions;
}

inline const Vec2 Sprite::GetBaseDimensions() const {
	return Vec2((float)Image::GetDimensions().width, (float)Image::GetDimensions().height);
}

inline void Sprite::SetScale(const Vec2& scale) {
	this->scale = scale;
	scaledOrigin = originalOrigin * scale;
	scaledDimensions = Vec2((float)Image::GetDimensions().width, (float)Image::GetDimensions().height) * scale;
}

inline const Vec2& Sprite::GetScale() const {
	return scale;
}
