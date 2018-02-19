#include "Level.h"
#include "Image.h"
#include "Game.h"

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
	layers.Append("Graphics/bg_layer_1.png", Vec3(-3000.0f, -2000.0f, 3.0f), Vec3(1.5f, 1.5f, 1.0f));
	layers.Append("Graphics/bg_layer_2.png", Vec3(0.0f, 850.0f, 1.0f), Vec3(0.8f, 0.8f, 0.8f));
	return true;
}

void Level::Unload() {
	layers.Clear();
}

BackgroundLayer::BackgroundLayer(const char* imageFilename, const Vec3& position, const Vec3& scale) {
	Image bgImage(imageFilename);

	if (bgImage.IsLoaded()) {
		texture = bgImage.CreateSDLTexture(game.GetRenderer());
	} else {
		texture = nullptr;
	}

	this->scale = scale * Vec3((float)bgImage.GetDimensions().width, (float)bgImage.GetDimensions().height, 1.0f) * position.z;
	this->position = position - scale / 2;
}

BackgroundLayer::~BackgroundLayer() {
	SDL_DestroyTexture(texture);
}

void BackgroundLayer::Render() const {
	const int cameraX = 0, cameraY = 0;
	SDL_Rect destRect = {position.x - cameraX, position.y - cameraY, scale.x, scale.y};

	game.GetCamera().RenderSprite(texture, position, scale.xy);
}