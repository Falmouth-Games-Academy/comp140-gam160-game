#pragma once
#include "Containers.h"
#include "Render.h"

// Box on the screen that contains debug text. Can be used at any time, but Render can only be called during game render
class DebugStringBox {
public:
	DebugStringBox(RenderScreen screen_, int x_, int y_, int width_, int height_) : screen(screen_), x(x_), y(y_), width(width_), height(height_) {};
	inline ~DebugStringBox();

public:
	// Renders the strings and resets the line pointer
	void Render(bool flushStrings = true);

	// Draws a string in the box and advances the line pointer
	void DrawString(const char* string);

	// Clears the deferred string list
	void ClearStrings();

private:
	RenderScreen screen;

	Array<char*> strings;

	int32 x, y;
	int32 width, height;
};

DebugStringBox::~DebugStringBox() {
	ClearStrings();
}