#pragma once
#include "Containers.h"

class ImageCache {
public:
	// Tries to obtain a cached image, or create a new one if none exist
	const class Image* ObtainImage(const char* filename);

	// Releases an image
	void ReleaseImage(const class Image* image);

	// Returns the number of cached images
	int GetNum() const;
	
	// Returns the number of references to the cached images
	int GetNumReferences() const;

	// Clears all images in the cache
	void Clear();


	void CleanupUnused();

private:
	static Cache<Image> cache;
};