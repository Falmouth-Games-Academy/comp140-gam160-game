#include "Level.h"
#include "Image.h"
#include "Game.h"

BackgroundLayer::BackgroundLayer(const char* imageFilename, const Vec3& position, const Vec3& scale) {
	Image bgImage(imageFilename);

	if (bgImage.IsLoaded()) {
		texture = bgImage.CreateSDLTexture(game.GetRenderer());
	} else {
		texture = nullptr;
	}

	this->position = position;
	this->scale = scale * Vec3(bgImage.GetDimensions().width, bgImage.GetDimensions().height, 1.0f);
}

BackgroundLayer::~BackgroundLayer() {
	SDL_DestroyTexture(texture);
}

void BackgroundLayer::Render() const {
	const int cameraX = 0, cameraY = 0;
	SDL_Rect destRect = {position.x - cameraX, position.y - cameraY, scale.x, scale.y};

	SDL_RenderCopyEx(game.GetRenderer(), texture, nullptr, &destRect, 0.0f, nullptr, SDL_FLIP_NONE);
}

Level::Level() {
	; // durr
}

Level::~Level() {
	Unload();
}

void Level::Render() const {
	// Render all layers for now
	for (const BackgroundLayer& layer : layers) {
		layer.Render();
	}
}

bool Level::Load() {
	layers.Append("Graphics/bg_layer_1.png", Vec3(0.0f, 0.0f, 10.0f));
	return true;
}

void Level::Unload() {
	layers.Clear();
}