#include <Windows.h>
#include <cstdio>

#include "Render.h"
#include "SDL.h"

bool RenderManager::Init(const char* windowTitle, int winX, int winY, int winWidth, int winHeight) {
	// Create window
	sdlWindow = SDL_CreateWindow(windowTitle, winX, winY, winWidth, winHeight, SDL_WINDOW_RESIZABLE);

	if (sdlWindow) {
		// Create render resources
		sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
		sdlTextSurface = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, textSurfaceWidth, textSurfaceHeight);

		// Init text render resources
		InitTextRenderer();

		// let's pretend that works cause we only have so much time here
		return true;
	} else {
		return false;
	}
}

bool RenderManager::InitTextRenderer() {
	// Create default font
	smallFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Trebuchet MS" /* Only the superior siege device */);
	largeFont = CreateFont(90, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Trebuchet MS");

	// Create text render DC
	textSurfaceDc = CreateCompatibleDC(NULL);

	// Create and select a bitmap into that DC
	int infiniteData[sizeof (BITMAPINFO) + 256 * sizeof (RGBQUAD)]; // lol (needed for bmiColors)
	BITMAPINFO& bi = *(BITMAPINFO*)infiniteData;
	memset(&bi, 0, sizeof (bi));

	bi.bmiHeader.biSize = sizeof (bi.bmiHeader);
	bi.bmiHeader.biWidth = textSurfaceWidth;
	bi.bmiHeader.biHeight = -textSurfaceHeight;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 8;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biClrUsed = 256;
	bi.bmiHeader.biClrImportant = 256;

	// Give the bitmap a basic monochrome gradient palette
	for (uint32 i = 0; i < 256; ++i) {
		bi.bmiColors[i] = RGBQUAD{(BYTE)i, (BYTE)i, (BYTE)i, 0};
	}

	// Setup GDI objects
	textSurfaceBitmap = CreateDIBSection(0, &bi, DIB_RGB_COLORS, (void**) &textSurfaceBits, nullptr, 0);

	SelectObject(textSurfaceDc, (HGDIOBJ) textSurfaceBitmap);
	SelectObject(textSurfaceDc, (HGDIOBJ) smallFont);
	SetBkColor(textSurfaceDc, RGB(0, 0, 0));
	SetTextColor(textSurfaceDc, RGB(255, 255, 255));

	// This was totally error checked and worked
	return true;
}

void RenderManager::RenderBegin() {
	// Clear the render screen
	SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);
	SDL_RenderClear(sdlRenderer);
}

void RenderManager::RenderEnd() {
	// Present everything that's been rendered!
	SDL_RenderPresent(sdlRenderer);
}

void RenderManager::Shutdown() {
	// Shutdown WinGDI stuff
	DeleteDC(textSurfaceDc);
	DeleteObject(smallFont);
	DeleteObject(largeFont);
	DeleteObject(textSurfaceBitmap);

	// Shutdown SDL components
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(sdlWindow);
	SDL_DestroyTexture(sdlTextSurface);
}

void RenderManager::RenderText(const char* string, int x, int y, const Colour& colour, bool isCentered, bool isLarge) {
	// Set the font
	if (isLarge) {
		SelectObject(textSurfaceDc, largeFont);
	} else {
		SelectObject(textSurfaceDc, smallFont);
	}

	// Calculate the size of the text
	RECT textBounds{0, 0, 0, 0};

	DrawTextEx(textSurfaceDc, const_cast<LPSTR>(string), -1, &textBounds, DT_CALCRECT, NULL);

	// Clamp the text bounds
	textBounds.right = Math::clamp((int)textBounds.right, 0, textSurfaceWidth);
	textBounds.bottom = Math::clamp((int)textBounds.bottom, 0, textSurfaceHeight);
	textBounds.top = Math::clamp((int)textBounds.top, 0, textSurfaceHeight);
	textBounds.left = Math::clamp((int)textBounds.left, 0, textSurfaceWidth);

	// Draw the text to the GDI DC
	DrawTextEx(textSurfaceDc, const_cast<LPSTR>(string), -1, &textBounds, 0, NULL);

	GdiFlush();

	// Don't try to render if the size is 0! This causes a crash!
	if (textBounds.right == 0 || textBounds.bottom == 0) {
		return;
	}

	// Copy the GDI text to the SDL texture
	SDL_Rect textureRect{0, 0, textBounds.right, textBounds.bottom};
	uint32* pixels;
	uint32 colourBits = ((colour.r << 16) | (colour.g << 8) | colour.b);
	int pitch;
	if (!SDL_LockTexture(sdlTextSurface, &textureRect, (void**)&pixels, &pitch)) {
		int textWidth = textBounds.right, textHeight = textBounds.bottom;
		for (int i = 0; i < textHeight; ++i) {
			uint32* pDest = &pixels[i * pitch / 4];
			uint8* pSrc = &textSurfaceBits[i * textSurfaceWidth];

			for (int i = 0; i < textWidth; ++i) {
				pDest[i] = (pSrc[i] << 24) | colourBits;
			}
		}

		SDL_UnlockTexture(sdlTextSurface);
	}

	// Render the SDL texture
	SDL_Rect src = {0, 0, textBounds.right, textBounds.bottom}, dest = {x, y, textBounds.right, textBounds.bottom};

	if (isCentered) {
		dest.x -= textBounds.right / 2;
		dest.y -= textBounds.bottom / 2;
	}

	SDL_SetTextureBlendMode(sdlTextSurface, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(sdlRenderer, sdlTextSurface, &src, &dest);
}

Dimensions2 RenderManager::GetScreenSize() {
	SDL_Rect sdlSize;

	SDL_RenderGetViewport(sdlRenderer, &sdlSize);
	return Dimensions2(sdlSize.w, sdlSize.h);
}

void RenderManager::RenderRectangle(bool filled) {
}