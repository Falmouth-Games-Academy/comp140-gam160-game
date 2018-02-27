#pragma once
#include "Containers.h"
#include "Math.h"
#include "Sprite.h"
#include "Object.h"

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
	
	// Renders collision boxes in the level layers
	void RenderCollisionBoxes() const;

	// Returns the closest layer at the given position
	class BackgroundLayer* GetLayerAtScreenPosition(const Vec2& position);

	// Returns layer list
	inline const Array<class BackgroundLayer>& GetLayers() const;
	inline Array<class BackgroundLayer>& GetLayers();

private:
	Array<class BackgroundLayer> layers;
};


// Background layer for level sections
class BackgroundLayer : public Object {
public:
	BackgroundLayer(int index, const char* imageFilename, const Vec3& position, const Vec2& scale = Vec2(1.0f, 1.0f));
	~BackgroundLayer();

	void Render() const;

	Object::Type GetType() const override {return Object::BackgroundLayer;}
};

inline Array<class BackgroundLayer>& Level::GetLayers() {
	return layers;
}

inline const Array<class BackgroundLayer>& Level::GetLayers() const {
	return layers;
}