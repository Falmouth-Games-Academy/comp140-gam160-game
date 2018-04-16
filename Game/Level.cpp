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

bool Level::Load(const char* filename) {
	// Unload old level
	Unload();

	// Load from the data file
	DataFile levelData;

	if (levelData.Load(filename)) {
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
					BackgroundLayer* layer = CreateLayer(backgroundImage, Vec3(0.0f, 0.0f, 0.0f));

					// Load position
					if (DataNode* position = node->GetNodeByName("pos")) {
						layer->SetPosition(position->GetValues().vec3Values[0]);
					}

					// Load scale
					if (DataNode* scale = node->GetNodeByName("scale")) {
						layer->SetScale(scale->GetValues().vec3Values[0].xy);
					}

					// Load collision box
					DataNode* collisionPos = node->GetNodeByName("cpos"), *collisionSize = node->GetNodeByName("csize");
					if (collisionPos && collisionSize) {
						layer->SetCollisionBox(Rect2(collisionPos->GetValues().vec3Values[0].xy, collisionSize->GetValues().vec3Values[0].xy));
						layer->SetCollisionFlags(Object::SolidOverlaps);
					}
				}
			}
		}

		// Create objects
		if (DataNode* objectNode = rootNode.GetNodeByName("objects")) {
			for (int i = 0, num = objectNode->GetNum(); i < num; ++i) {
				DataNode* node = objectNode->GetValues().nodeValues[i];
				Object* object;

				if (DataNode* type = node->GetNodeByName("type")) {
					object = game.SpawnObject((Object::Type) type->GetValues().intValues[0]);
				} else {
					// Object type wasn't found
					continue;
				}

				// Load position
				if (DataNode* position = node->GetNodeByName("pos")) {
					object->SetPosition(position->GetValues().vec3Values[0]);
				}

				// Load rotation
				if (DataNode* rotation = node->GetNodeByName("rot")) {
					object->SetRotation(rotation->GetValues().floatValues[0]);
				}

				// Load scale
				if (DataNode* scale = node->GetNodeByName("scale")) {
					object->SetScale(scale->GetValues().vec3Values[0].xy);
				}

				// Set spawn info
				object->SetSpawnInfo(object->GetPosition(), true);
			}
		}

		// Set player start
		if (DataNode* playerStartNode = rootNode.GetNodeByName("playerstart")) {
			playerStart = playerStartNode->GetValues().vec3Values[0];
		} else {
			playerStart = Vec3(0.0f, 0.0f, 1.0f);
		}
	} else {
		return false;
	}

	// Respawn the player
	game.RespawnPlayer();

	return true;
}

bool Level::Save(const char* filename) const {
	DataFile data;
	DataNode& rootNode = data.GetRootNode();

	// Store the player start
	DataNode* playerStartNode = rootNode.AddNode(DataNode::Vector3, "playerstart", 1);
	playerStartNode->GetValues().vec3Values[0] = playerStart;

	// Store the level layer data
	DataNode* layerPackNode = rootNode.AddNode(DataNode::Node, "background", 0);

	for (const BackgroundLayer& layer : game.GetLevel().GetLayers()) {
		DataNode* layerNode = layerPackNode->AddNode(DataNode::Node, "layer", 0);

		// Write sprite
		if (const SpriteFrame* frame = layer.GetSprite().GetCurrentFrame()) {
			if (const Image* image = frame->GetImage()) {
				if (const char* imageFilename = image->GetFilename()) {
					// So many safety checks zomg
					DataNode* spriteNode = layerNode->AddNode(DataNode::String, "sprite", 0);
					spriteNode->SetValueAsString(imageFilename);
				}
			}
		}

		// Write position
		DataNode* positionNode = layerNode->AddNode(DataNode::Vector3, "pos", 1);
		positionNode->GetValues().vec3Values[0] = layer.GetPosition();

		// Write scale
		DataNode* scaleNode = layerNode->AddNode(DataNode::Vector3, "scale", 1);
		scaleNode->GetValues().vec3Values[0] = layer.GetSprite().GetScale();

		// Write collision (if solid)
		if (layer.GetCollisionFlags()) {
			DataNode* cPosNode = layerNode->AddNode(DataNode::Vector3, "cpos", 1);
			cPosNode->GetValues().vec3Values[0] = layer.GetCollisionBox().position;

			DataNode* cSizeNode = layerNode->AddNode(DataNode::Vector3, "csize", 1);
			cSizeNode->GetValues().vec3Values[0] = layer.GetCollisionBox().size;
		}
	}

	// Store the objects
	DataNode* objectPackNode = rootNode.AddNode(DataNode::Node, "objects", 0);
	for (Object* object : game.GetObjects()) {
		if (object->IsPersistent()) {
			DataNode* objectNode = objectPackNode->AddNode(DataNode::Node, "object", 0);

			// Write object type
			DataNode* typeNode = objectNode->AddNode(DataNode::Int, "type", 1);
			typeNode->GetValues().intValues[0] = (int)object->GetType();

			// Write object spawn position
			DataNode* positionNode = objectNode->AddNode(DataNode::Vector3, "pos", 1);
			positionNode->GetValues().vec3Values[0] = object->GetSpawnPosition();

			// Write object spawn rotation
			DataNode* rotationNode = objectNode->AddNode(DataNode::Float, "rot", 1);
			rotationNode->GetValues().floatValues[0] = object->GetRotation();

			// Write object scale
			DataNode* scaleNode = objectNode->AddNode(DataNode::Vector3, "scale", 1);
			scaleNode->GetValues().vec3Values[0] = object->GetSprite().GetScale();
		}
	}

	// Save the level data
	return data.Save(filename);
}

void Level::Unload() {
	// Todo: This doesn't really belong here. What do do I?
	game.ClearObjects();

	ClearLayers();
}

BackgroundLayer* Level::CreateLayer(const char* imageFilename, const Vec3& position, const Vec2& scale) {
	BackgroundLayer& layer = layers.Append(imageFilename, position, Vec2(1.5f, 1.5f));
	
	return &layer;
}

void Level::DestroyLayer(BackgroundLayer* layer) {
	// Find the layer in the layer list and strip it out
	for (int i = 0; i < layers.GetNum(); ++i) {
		if (&layers[i] == layer) {
			layers.RemoveByIndex(i);
			break;
		}
	}
}

void Level::ClearLayers() {
	// Call Destroy on all layers just in case they have any cleanup to do
	for (BackgroundLayer& layer : layers) {
		layer.Destroy();
	}

	// Clear the layer list
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

BackgroundLayer::BackgroundLayer(const char* imageFilename, const Vec3& position, const Vec2& scale) : Object() {
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