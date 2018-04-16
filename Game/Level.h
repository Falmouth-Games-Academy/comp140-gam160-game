#pragma once
#include "Containers.h"
#include "Math.h"
#include "Sprite.h"
#include "Object.h"

class Level {
public:
	Level();
	~Level();

public:
	// Loads the level structure from a file, additionally loading the images, and returns whether wholly successful
	bool Load(const char* filename);

	// Saves the level structure into a file of the given file name, including the layer image file names, and returns whether wholly successful
	bool Save(const char* filename) const;

	// Frees the level
	void Unload();

public:
	// Renders the level background
	void Render() const;
	
	// Renders collision boxes in the level layers
	void RenderCollisionBoxes() const;

public:
	// Gets the player start position
	inline const Vec3& GetPlayerStart();

public:
	// Creates a new background layer
	class BackgroundLayer* CreateLayer(const char* imageFilename, const Vec3& position, const Vec2& scale = Vec2(1.0f, 1.0f));

	// Destroys a background layer
	void DestroyLayer(BackgroundLayer* layer);

	// Destroys all layers
	void ClearLayers();

	// Returns the closest layer at the given position
	class BackgroundLayer* GetLayerAtScreenPosition(const Vec2& position);

	// Returns layer list
	inline const Array<class BackgroundLayer>& GetLayers() const;
	inline Array<class BackgroundLayer>& GetLayers();

private:
	// Array of background layer objects that comprise the level background
	Array<class BackgroundLayer> layers;

	// The starting position of the player
	Vec3 playerStart;
};


// Background layer for level sections
class BackgroundLayer : public Object {
public:
	// Loads a background layer with an image and position
	BackgroundLayer(const char* imageFilename, const Vec3& position, const Vec2& scale = Vec2(1.0f, 1.0f));
	~BackgroundLayer();

	BackgroundLayer(BackgroundLayer&& moved) = default;

public:
	void Render() const;

	Object::Type GetType() const override {return BackgroundLayerType;}
};

inline Array<class BackgroundLayer>& Level::GetLayers() {
	return layers;
}

inline const Array<class BackgroundLayer>& Level::GetLayers() const {
	return layers;
}

inline const Vec3& Level::GetPlayerStart() {
	return playerStart;
}