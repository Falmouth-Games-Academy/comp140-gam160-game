#include "ImageCache.h"
#include "Image.h"

Cache<Image> ImageCache::cache;

const Image* ImageCache::ObtainImage(const char* filename) {
	// Finds an image in the cache
	Image* image = cache.Retrieve(filename);

	if (image) {
		// Return it if found
		return image;
	} else {
		// Otherwise create a new image
		image = new Image(filename);

		if (image->IsLoaded()) {
			// Only add to the cache if the image was successfully loaded
			cache.Bind(filename, image);
			return image;
		} else {
			delete image;
			return nullptr;
		}
	}
}

void ImageCache::ReleaseImage(const Image* image) {
	cache.Release(image);
}

int ImageCache::GetNum() const {
	return cache.GetNum();
}

int ImageCache::GetNumReferences() const {
	return -1; // todo
}
