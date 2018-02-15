#include "Types.h"
#include "Game.h"

#include "Graph.h"

#include <windows.h> // I didn't wanna do this, but font rendering need.
#include <initializer_list>

// The global variable... KILL IT WITH FIRE! LEST THE STUPID PROGRAMMERS BERAK SOMETHING!!!!
Game game;

const char* port = "COM5";

static HFONT defaultFont;
HDC textSurfaceDc;
HBITMAP textSurfaceBitmap;
uint8* textSurfaceBits;

void Game::Init() {
	AllocConsole();
	freopen("conout$","w",stdout);

	printf("Init SDL\n");
	SDL_Init(SDL_INIT_VIDEO);

	printf("Init windows\n");
	// Create windows and renderers
	struct ScreenInitInfo {RenderScreen screen; const char* windowTitle; int x, y; int width, height;};
	SDL_Rect screenBounds;
	SDL_GetDisplayBounds(0, &screenBounds);
	int screenCentreX = screenBounds.w / 2, screenCentreY = screenBounds.h / 2;

	for (const ScreenInitInfo& init : {ScreenInitInfo{Main, "Epic Handventure", -2, -1, 640, 480}, ScreenInitInfo{Debug, "Debug graphs", 0, -1, 600, 600}}) {
		sdlWindows[init.screen] = SDL_CreateWindow(init.windowTitle, screenCentreX + init.x * (init.width / 2), screenCentreY + init.y * (init.height / 2), 
												   init.width, init.height, SDL_WINDOW_RESIZABLE);
	
		if (sdlWindows[init.screen]) {
			sdlRenderers[init.screen] = SDL_CreateRenderer(sdlWindows[init.screen], -1, SDL_RENDERER_ACCELERATED);
			sdlTextSurfaces[init.screen] = SDL_CreateTexture(sdlRenderers[init.screen], SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 500, 500);
		}
	}

	printf("Creating additional text rendering assets that nobody wants in their life ever\n");
	// Create default font
	defaultFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");

	// Create text render DC
	textSurfaceDc = CreateCompatibleDC(NULL);

	// Create and select a bitmap into that DC
	int infiniteData[4000];
	BITMAPINFO& bi = *(BITMAPINFO*)infiniteData;
	memset(&bi, 0, sizeof (bi));

	bi.bmiHeader.biSize = sizeof (bi.bmiHeader);
	bi.bmiHeader.biWidth = 500;
	bi.bmiHeader.biHeight = -500;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 8;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biClrUsed = 256;
	bi.bmiHeader.biClrImportant = 256;
	
	for (BYTE i = 0; i < 255; ++i) {
		bi.bmiColors[i] = RGBQUAD{i, i, i, 0};
	}

	textSurfaceBitmap = CreateDIBSection(0, &bi, DIB_RGB_COLORS, (void**) &textSurfaceBits, nullptr, 0);

	SelectObject(textSurfaceDc, (HGDIOBJ) textSurfaceBitmap);
	SelectObject(textSurfaceDc, (HGDIOBJ) defaultFont);
	SetBkColor(textSurfaceDc, RGB(0, 0, 0));
	SetTextColor(textSurfaceDc, RGB(255, 255, 255));

	// Initialise Arduino if possible
	printf("Init Arduino\n");

	arduino = new Serial(port);

	if (arduino->IsConnected())
		printf("Cereal has been made!\n");
	else
		printf("Arduino couldn't open. Cereal killer on the loose!!\n");

	printf("Init complete\n");
}

void Game::Shutdown() {
	// Shutdown Arduino
	delete arduino;

	// Shutdown dumb WinGDI stuff
	DeleteDC(textSurfaceDc);
	DeleteObject(defaultFont);
	DeleteObject(textSurfaceBitmap);

	// Shutdown SDL components
	for (int i = 0; i < NumRenderScreens; ++i) {
		SDL_DestroyRenderer(sdlRenderers[i]);
		SDL_DestroyWindow(sdlWindows[i]);
		SDL_DestroyTexture(sdlTextSurfaces[i]);
	}

	SDL_Quit();
}

unsigned char incomingData[256] = "";
void Game::Update(float deltaTime) {
	// Update gesture input
	gesture.Update();

	// Update player
	player.Update(deltaTime);
}

void Game::Render() {
	// Start rendering
	for (int i = 0; i < RenderScreen::NumRenderScreens; ++i) {
		SDL_SetRenderDrawColor(sdlRenderers[i], 255, 255, 255, 255);
		SDL_RenderClear(sdlRenderers[i]);
	}
	
	// Render gesture debug stuff
	gesture.Render();

	// Render player
	player.Render();

	// Render text
	// Draw the text onto that bitmap
	const int textWidth = 500, textHeight = 40;
	RECT r = {0, 0, textWidth, textHeight};
	
	//FillRect(drawDc, &r, (HBRUSH) BLACK_BRUSH);
	
	char str[100];
	sprintf(str, "FPS: %i", (int) (1.0f / deltaTime));
	SDL_SetWindowTitle(sdlWindows[Main], str);

	RenderText(str, 0, 0);

	// Finish rendering
	for (int i = 0; i < RenderScreen::NumRenderScreens; ++i) {
		SDL_RenderPresent(sdlRenderers[i]);
	}
}

void Game::Run() {
	// Initialise engine
	Init();

	// Spawn the player
	player.Spawn();

	// Begin main loop!
	SDL_Event sdlEvent;
	bool appIsRunning = true;
	uint64 performanceFrequency = SDL_GetPerformanceFrequency();
	uint64 lastPerformanceCounter = SDL_GetPerformanceCounter();

	while (appIsRunning) {
		// Calculate frame times
		uint64 currentPerformanceCounter = SDL_GetPerformanceCounter();
		deltaTime = (float) (currentPerformanceCounter - lastPerformanceCounter) / performanceFrequency;
		frameTime = (uint32) (currentPerformanceCounter * 1000 / performanceFrequency) - startFrameTime;

		if (!startFrameTime) {
			// If this is the first frame, set the start frame time here (here instead of the init functions, for supah precision)
			startFrameTime = frameTime;
			frameTime = 0;
			deltaTime = 0.0f;
		}

		// Update the input system before sending it input events--this clears boops
		input.Update();

		// Run SDL event loop
		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_WINDOWEVENT) {
				// Check if the window has been closed
				if (sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE)
					appIsRunning = false;
			} else if (sdlEvent.type == SDL_KEYDOWN || sdlEvent.type == SDL_KEYUP) {
				// Defer the input event to the input manager
				input.OnInputEvent(sdlEvent);
			}
		}

		// Update the game!
		Update(deltaTime);

		// Render the game!
		Render();

		// temp: Frame limit
		SDL_Delay(2);

		// Timer update
		lastPerformanceCounter = currentPerformanceCounter;
	}

	// Game has ended
	// Cleanup
	Shutdown();
}

void Game::RenderText(const char* string, int x, int y, RenderScreen screen) {
	// Get text bounds
	RECT r{0, 0, 0, 0};

	DrawTextEx(textSurfaceDc, const_cast<LPSTR>(string), -1, &r, DT_CALCRECT, NULL);

	// Draw the text to the GDI DC
	DrawTextEx(textSurfaceDc, const_cast<LPSTR>(string), -1, &r, 0, NULL);

	GdiFlush();

	// Don't try to render if the size is 0! This causes a crash!
	if (r.right == 0 || r.bottom == 0) {
		return;
	}

	// Copy the GDI text to the SDL texture
	SDL_Rect textureRect{0, 0, r.right, r.bottom};
	uint32* pixels;
	int pitch;
	if (!SDL_LockTexture(sdlTextSurfaces[screen], &textureRect, (void**)&pixels, &pitch)) {
		int textWidth = r.right, textHeight = r.bottom;
		for (int i = 0; i < textHeight; ++i) {
			uint32* pDest = &pixels[i * pitch / 4];
			uint8* pSrc = &textSurfaceBits[i * 500];

			for (int i = 0; i < textWidth; ++i) {
				pDest[i] = pSrc[i] << 24;
			}
		}

		SDL_UnlockTexture(sdlTextSurfaces[screen]);
	}

	// Render the SDL texture
	SDL_Rect src = {0, 0, r.right, r.bottom}, dest = {x, y, r.right, r.bottom};

	SDL_SetTextureBlendMode(sdlTextSurfaces[screen], SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(sdlRenderers[screen], sdlTextSurfaces[screen], &src, &dest);
}