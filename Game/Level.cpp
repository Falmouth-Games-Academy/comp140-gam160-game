#include "Level.h"
#include "Image.h"
#include "Game.h"

#include "DataFile.h"

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
	BackgroundLayer& bak = layers.Append(layers.GetNum(), "Graphics/bg_layer_1.png", Vec3(0.0f, 0.0f, 3.0f), Vec2(1.5f, 1.5f));
	BackgroundLayer& platform = layers.Append(layers.GetNum(), "Graphics/bg_layer_2.png", Vec3(0.0f, 500.0f, 1.0f), Vec2(0.8f, 0.8f));

	bak.SetPosition(Vec3(bak.GetSize().x * -0.5f , bak.GetSize().y * -0.5f, 3.0f));

	platform.SetCollision(&Rect2(0.0f, 60.0f, 2192.0f, 384.0f));

	// Test: Load from the data file
	DataFile levelData;

	if (levelData.Load("./maps/map1.hdf")) {
		DataNode& rootNode = levelData.GetRootNode();

		// Create background layers
		if (DataNode* layerNode = rootNode.GetNodeByName("background")) {
			for (int i = 0, num = layerNode->GetNum(); i < num; ++i) {
				DataNode* node = layerNode->GetValues().nodeValues[i];

				// Load sprite image
				const char* backgroundImage = nullptr;
				if (DataNode* sprite = node->GetNodeByName("sprite")) {
					backgroundImage = sprite->GetValueAsString();
				}

				if (backgroundImage) {
					// Create the new layer
					BackgroundLayer& layer = layers.Append(layers.GetNum(), "graphics/bg_layer_1.png", Vec3(0.0f, 0.0f, 0.0f), Vec2(1.0f, 1.0f));

					// Load position
					if (DataNode* position = node->GetNodeByName("pos")) {
						layer.SetPosition(position->GetValues().vec3Values[0]);
					}

					// Load scale
					if (DataNode* scale = node->GetNodeByName("scale")) {
						layer.SetScale(scale->GetValues().vec3Values[0].xy);
					}
				}
			}
		}
	}

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
	sprite.LoadFrame(imageFilename);
	sprite.SetScale(scale);
	this->position = position;
}

BackgroundLayer::~BackgroundLayer() {
	return;
}

void BackgroundLayer::Render() const {
	game.GetCamera().RenderSprite(sprite, position);
}