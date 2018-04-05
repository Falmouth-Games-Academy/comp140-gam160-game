#include "Sprite.h"
#include "Game.h"
#include "String.h"

SpriteFrame::~SpriteFrame() {
	ClearTextureCache();

	if (sourceImage) {
		game.GetImageCache().ReleaseImage(sourceImage);
	}
}

void SpriteFrame::Load(const char* filename, const Vec2& origin, const Vec2& scale) {
	// Clear the textures
	ClearTextureCache();

	// Unload the previous image, if it exists
	if (sourceImage) {
		game.GetImageCache().ReleaseImage(sourceImage);
	}

	// Try and load the parent image
	sourceImage = game.GetImageCache().ObtainImage(filename);

	// Set the origin and scale anyway
	this->baseOrigin = origin;
	this->scale = scale;

	// Update actual dimensions etc if the image successfully loaded
	if (IsLoaded()) {
		scaledDimensions = GetBaseDimensions() * scale;
		scaledOrigin = origin * scale;
	} else {
		scaledDimensions = Vec2(0.0f, 0.0f);
		scaledOrigin = Vec2(0.0f, 0.0f);
	}
}

SDL_Texture* SpriteFrame::GetSDLTexture(RenderScreen screen) const {
	// Try to create the texture if it doesn't already exist
	if (!textures[screen] && sourceImage) {
		textures[screen] = sourceImage->CreateSDLTexture(game.GetRenderer(screen));
	}

	return textures[screen];
}

void SpriteFrame::ClearTextureCache() {
	for (SDL_Texture* texture : textures) {
		if (texture) {
			SDL_DestroyTexture(texture);
		}
	}
}

bool Sprite::LoadFrames(const char* filename, int numFrames, const Vec2& origin, const Vec2& scale) {
	int numSuccessfullyLoaded = 0;

	// Try to load a bunch of frames using the given format
	for (int i = 0; i < numFrames; ++i) {
		// Create the frame
		SpriteFrame* newFrame = new SpriteFrame(StaticString<MAX_PATH>::FromFormat("%s_%i.png", filename, i), origin, scale);

		// See if it loaded successfully
		if (newFrame->IsLoaded()) {
			// It loaded! Add it to the array
			frames.Append() = newFrame;

			++numSuccessfullyLoaded;
		} else {
			// Cleanup
			delete newFrame;
		}
	}

	// Return whether all frames were loaded
	return numSuccessfullyLoaded == numFrames;
}

void Sprite::RemoveFrame(int frameIndex) {
	frames.RemoveByIndex(frameIndex);
}

void Sprite::TickAnimation(float deltaTime) {
	currentFrame = Math::circleclamp(currentFrame + frameRate * deltaTime, (float)frames.GetNum());
}

void Sprite::SetScale(const Vec2& scale) {
	for (SpriteFrame* frame : frames) {
		frame->SetScale(scale);
	}
}