#pragma once
#include "Math.h" // Dimensions2
#include "Types.h"
#include "SDL.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

// Render stuff
enum RenderScreen : uint8 {
	Main,
	Debug,
	NumRenderScreens
};

class RenderManager {
public:
	// SDL component getters
	struct SDL_Window* GetSDLWindow();
	struct SDL_Renderer* GetSDLRenderer();
	struct SDL_Texture* GetSDLTextSurface();

public:
	// Init and shutdown functions: must be called for the renderer to work
	// Creates a window and renderer at a given position.
	bool Init(const char* windowTitle, int winX, int winY, int winWidth, int winHeight);

	// Frees renderer and resources
	void Shutdown();

private:
	bool InitTextRenderer();

public:
	// Render functions
	void RenderBegin();
	void RenderEnd();

	// Renders text at the default size
	void RenderText(const char* text, int x, int y, bool isCentered = false, bool isLarge = false);
	void RenderRectangle(bool filled = false);

	Dimensions2 GetScreenSize();

private:
	// Game renderers, one for each window
	struct SDL_Renderer* sdlRenderer;

	// Game windows
	struct SDL_Window* sdlWindow;

	// Surface for text rendering
	struct SDL_Texture* sdlTextSurface;

	// Fonts
	HFONT smallFont;
	HFONT largeFont;

	// Resources for text render
	HDC textSurfaceDc;
	HBITMAP textSurfaceBitmap;
	uint8* textSurfaceBits;

	// Default text surface size (this defines the maximum size you can render text along)
	const int textSurfaceSize = 1024;
};

inline struct SDL_Window* RenderManager::GetSDLWindow() {
	return sdlWindow;
}

inline struct SDL_Renderer* RenderManager::GetSDLRenderer() {
	return sdlRenderer;
}

inline struct SDL_Texture* RenderManager::GetSDLTextSurface() {
	return sdlTextSurface;
}