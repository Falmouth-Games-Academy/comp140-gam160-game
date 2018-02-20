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
	//layers.Append("Graphics/bg_layer_1.png", Vec3(-3000.0f, -2000.0f, 3.0f), Vec2(1.5f, 1.5f));
	//layers.Append("Graphics/bg_layer_2.png", Vec3(0.0f, 850.0f, 1.0f), Vec2(0.8f, 0.8f));

	BackgroundLayer& bak = layers.Append("Graphics/bg_layer_1.png", Vec3(0.0f, 0.0f, 3.0f), Vec2(1.5f, 1.5f));
	layers.Append("Graphics/bg_layer_2.png", Vec3(0.0f, 0.0f, 1.0f), Vec2(0.8f, 0.8f));

	bak.SetPosition(Vec3(bak.GetSize().x * -0.5f , bak.GetSize().y * -0.5f, 3.0f));
	return true;
}

void Level::Unload() {
	layers.Clear();
}

BackgroundLayer* Level::GetLayerAtPosition(const Vec3& position) {
	Vec3 worldPosition = game.GetCamera().ScreenToWorld(position);

	// Find a layer where worldPosition is within its boundaries
	for (BackgroundLayer& layer : layers) {
		if (worldPosition.xy.IsWithin(layer.GetPosition().xy, layer.GetPosition().xy + layer.GetSize())) {
			return &layer;
		}
	}

	return nullptr;
}

BackgroundLayer::BackgroundLayer(const char* imageFilename, const Vec3& position, const Vec2& scale) : 
		sprite(imageFilename, Vec2(0.0f, 0.0f), scale) {

	this->position = position;
}

BackgroundLayer::~BackgroundLayer() {
	return;
}

void BackgroundLayer::Render() const {
	game.GetCamera().RenderSprite(sprite, position);
	//game.GetCamera().RenderSprite(texture, position, scale.xy);
}