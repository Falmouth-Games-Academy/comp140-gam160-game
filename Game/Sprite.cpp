#include "Sprite.h"
#include "Game.h"

Sprite::~Sprite() {
	for (SDL_Texture* texture : textures) {
		if (texture) {
			SDL_DestroyTexture(texture);
		}
	}
}

void Sprite::Load(const char* filename, const Vec2& origin, const Vec2& scale) {
	// Try and load the parent image
	Image::Load(filename);
	
	// Set the origin and scale anyway
	this->originalOrigin = origin;
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

SDL_Texture* Sprite::GetSDLTexture(RenderScreen screen) const {
	// Try to create the texture if it doesn't already exist
	if (!textures[screen]) {
		textures[screen] = CreateSDLTexture(game.GetRenderer(screen));
	}

	return textures[screen];
}