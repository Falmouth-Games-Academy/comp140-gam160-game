#pragma once
#include "Containers.h"
#include "Math.h"
#include "Sprite.h"

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
	
	// Returns the closest layer at the given position
	class BackgroundLayer* GetLayerAtScreenPosition(const Vec2& position);

	// Returns layer list
	inline const Array<class BackgroundLayer>& GetLayers() const;
	inline Array<class BackgroundLayer>& GetLayers();

private:
	Array<class BackgroundLayer> layers;
};


// Background layer for level sections
class BackgroundLayer {
public:
	BackgroundLayer(int index, const char* imageFilename, const Vec3& position, const Vec2& scale = Vec2(1.0f, 1.0f));
	~BackgroundLayer();

	void Render() const;
	
	inline void SetPosition(const Vec3& position);

	inline int GetIndex() const;
	inline const Vec3& GetPosition() const;
	inline const Vec2 GetSize() const;

private:
	// Layer image
	Sprite sprite;

	// Position of the layer, including z, which is depth
	Vec3 position;

	// Index of the layer in the parent level's layer list
	int32 index;

private:
	inline void SetIndex(int newIndex); // Do not call unless you're Level!
};

inline Array<class BackgroundLayer>& Level::GetLayers() {
	return layers;
}

inline const Array<class BackgroundLayer>& Level::GetLayers() const {
	return layers;
}

inline void BackgroundLayer::SetIndex(int newIndex) {
	index = newIndex;
}

inline int BackgroundLayer::GetIndex() const {
	return index;
}

inline const Vec3& BackgroundLayer::GetPosition() const {
	return position;
}

inline const Vec2 BackgroundLayer::GetSize() const {
	return sprite.GetDimensions();
}

inline void BackgroundLayer::SetPosition(const Vec3& position) {
	this->position = position;
}