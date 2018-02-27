#include "Image.h"

#include <SDL.h>
#include <wchar.h> // mbstowcs

#include <Windows.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

int32 Image::numRefs = 0;

Image::Image(const char* filename) : pixels(nullptr) {
	// Add global ref
	Image::AddRef();

	// Try to load the image
	Load(filename);
}

Image::Image(const Image& other) : dimensions(other.dimensions), pitch(other.pitch) {
	// Add reference
	Image::AddRef();

	// Copy the pixels over
	pixels = new uint32[pitch * dimensions.height / 4];
	memcpy(pixels, other.pixels, pitch * dimensions.height);
}

Image::~Image() {
	Image::RemoveRef();

	delete pixels;
}

bool Image::Load(const char* filename) {
	// Unload the previous image, if one exists
	delete pixels;
	pixels = nullptr;

	// Try to load the file into a GDIPlus image
	wchar_t wideName[280];
	mbstowcs_s(nullptr, wideName, filename, 280);

	Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(wideName);

	// Copy the image data to this->pixels if possible
	if (bitmap) {
		// Lock the bitmap bits
		Gdiplus::BitmapData data;
		if (!bitmap->LockBits(&Gdiplus::Rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight()), Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data)) {
			// If successful, copy the bitmap bits to this->pixels
			this->pixels = new uint32[bitmap->GetWidth() * bitmap->GetHeight()];
			memcpy(pixels, data.Scan0, bitmap->GetWidth() * bitmap->GetHeight() * 4);

			// Update the image dimensions and pitch
			this->dimensions = {(int32)bitmap->GetWidth(), (int32)bitmap->GetHeight()};
			this->pitch = data.Width * 4;

			bitmap->UnlockBits(&data);
		}
	}

	// Free the bitmap
	delete bitmap;

	// Return whether the load was evidently successful
	return (pixels != nullptr);
}

SDL_Texture* Image::CreateSDLTexture(SDL_Renderer* renderer) const {
	// Try to create a surface from the pixel data
	if (SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, dimensions.width, dimensions.height, 32, pitch, 
														0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000))
	{
		// Then try to create a texture from the surface
		if (SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface)) {
			// Success!
			SDL_FreeSurface(surface);
			return texture;
		} else {
			SDL_FreeSurface(surface);
		}
	}

	return nullptr;
}

static ULONG_PTR gdiplusToken;

void Image::AddRef() {
	if (numRefs == 0) {
		// Initialise GDI plus
		Gdiplus::GdiplusStartupInput useless;
		Gdiplus::GdiplusStartup(&gdiplusToken, &useless, nullptr);
	}

	// Increment global image reference counter
	++numRefs;
}

void Image::RemoveRef() {
	--numRefs;

	if (numRefs == 0) {
		// Uninitialise GDI plus
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}
}