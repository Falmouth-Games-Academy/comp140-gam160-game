#include "Game.h"
#include "DebugStringBox.h"

#include <cassert>
#include <cstring>

// Renders the strings and resets the line pointer
void DebugStringBox::Render(bool flushStrings) {
	int currentTextY = 0;

	for (char* string : strings) {
		game.GetRenderer(screen).RenderText(string, x, y + currentTextY);
		currentTextY += 20;
	}

	if (flushStrings) {
		ClearStrings();
	}
}

// Draws a string in the box and advances the line pointer
void DebugStringBox::DrawString(const char* string) {
	assert(strings.GetNum() < 30); // Idiot programmer probably forgot to flush the string buffer, who's writing this!?

	const int stringSize = strlen(string) + 1;
	char* deferredString = strings.Append(new char[stringSize]);

	memcpy(deferredString, string, stringSize);
}

// Clears the deferred string list
void DebugStringBox::ClearStrings() {
	for (char* string : strings) {
		delete[] string;
	}

	strings.Clear();
}