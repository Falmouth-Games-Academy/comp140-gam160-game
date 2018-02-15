#pragma once
#include "Containers.h"
#include "Math.h"

class Level {
public:
	Level();
	~Level();

	// Loads the level into memory and returns whether the load was wholly successful
	bool Load();

	// Frees the level
	void Unload();

	// Renders the level background
	void Render() const;

private:
	Array<class BackgroundLayer> layers;
};


// Background layer for level sections
class BackgroundLayer {
public:
	BackgroundLayer(const char* imageFilename, const Vec3& position, const Vec3& scale = Vec3(1.0f, 1.0f, 1.0f));
	~BackgroundLayer();

	void Render() const;

private:
	struct SDL_Texture* texture;

	// Position of layer, including z, which is depth
	Vec3 position;

	// Scale cuz that might change over time
	Vec3 scale;
};