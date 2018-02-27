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

void Level::RenderCollisionBoxes() const {
	for (const BackgroundLayer& layer : layers) {
		layer.RenderCollisionBox();
	}
}

bool Level::Load() {
	//layers.Append("Graphics/bg_layer_1.png", Vec3(-3000.0f, -2000.0f, 3.0f), Vec2(1.5f, 1.5f));
	//layers.Append("Graphics/bg_layer_2.png", Vec3(0.0f, 850.0f, 1.0f), Vec2(0.8f, 0.8f));

	BackgroundLayer& bak = layers.Append(layers.GetNum(), "Graphics/bg_layer_1.png", Vec3(0.0f, 0.0f, 3.0f), Vec2(1.5f, 1.5f));
	layers.Append(layers.GetNum(), "Graphics/bg_layer_2.png", Vec3(0.0f, 0.0f, 1.0f), Vec2(0.8f, 0.8f));

	bak.SetPosition(Vec3(bak.GetSize().x * -0.5f , bak.GetSize().y * -0.5f, 3.0f));
	return true;
}

void Level::Unload() {
	layers.Clear();
}

BackgroundLayer* Level::GetLayerAtScreenPosition(const Vec2& position) {
	float closestZ = 0.0f;
	BackgroundLayer* returnLayer = nullptr;
	const Camera& camera = game.GetCamera();

	// Find a layer where worldPosition is within its boundaries
	for (BackgroundLayer& layer : layers) {
		Vec3 worldPosition = game.GetCamera().ScreenToWorld(Vec3(position, layer.GetPosition().z));

		if (worldPosition.xy.IsWithin(layer.GetPosition().xy, layer.GetPosition().xy + layer.GetSize())) {
			if (layer.GetPosition().z < closestZ || !closestZ) {
				closestZ = layer.GetPosition().z;
				returnLayer = &layer;
			}
		}
	}

	return returnLayer;
}

BackgroundLayer::BackgroundLayer(int index_, const char* imageFilename, const Vec3& position, const Vec2& scale) : Object() {
	sprite.Load(imageFilename);
	sprite.SetScale(scale);
	this->position = position;
}

BackgroundLayer::~BackgroundLayer() {
	return;
}

void BackgroundLayer::Render() const {
	game.GetCamera().RenderSprite(sprite, position);
}