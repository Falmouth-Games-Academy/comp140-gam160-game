#include "Game.h"
#include "Graph.h"

#include <SDL_timer.h>

void Graph::PushValue(int value) {
	uint32 timestamp = game.GetFrameTime();

	if (numValues == maxNumValues) {
		// Array is full so shift the existing values to the left
		for (int i = 0; i < maxNumValues - 1; i++) {
			values[i] = values[i + 1];
			valueTimestamps[i] = valueTimestamps[i + 1];
		}

		values[maxNumValues - 1] = value;
		valueTimestamps[maxNumValues - 1] = timestamp;
	} else {
		// Array isn't full yet; no need to shift values
		values[numValues] = value;
		valueTimestamps[numValues] = timestamp;
		++numValues;
	}

	// Test: Delete old bars
	for (int i = 0; i < bars.GetNum(); ++i) {
		if (bars[i].time < timestamp - renderTimeRange) {
			bars.RemoveByIndex(i--);
		}
	}

	UpdateAutoScale();
}

void Graph::PushValueAtTime(int value, uint32 timestamp) {
	int insertIndex = -1;
	bool doReplace = false;

	// Find where to insert it
	for (int i = maxNumValues - 1; i >= 0; --i) {
		if (valueTimestamps[i] <= timestamp) {
			if (valueTimestamps[i] == timestamp)
				doReplace = true;

			insertIndex = i;
			break;
		}
	}

	if (insertIndex == -1)
		return; // No valid location found
	
	if (!doReplace) {
		// Shift the existing values to the left
		for (int i = 0; i < insertIndex; i++) {
			values[i] = values[i + 1];
			valueTimestamps[i] = valueTimestamps[i + 1];
		}
	}

	// Insert this value
	values[insertIndex] = value;
	valueTimestamps[insertIndex] = timestamp;

	// Increment numValues if necessary
	if (numValues < maxNumValues && !doReplace) {
		++numValues;
	}

	UpdateAutoScale();
}

void Graph::SetRenderValueRange(MinMax<int32> scale) {
	renderMin = scale.min;
	renderMax = scale.max;
}

MinMax<int32> Graph::GetRenderValueRange() const {
	return MinMax<int32>(renderMin, renderMax);
}

void Graph::SetRenderPosition(int x, int y) {
	renderX = x;
	renderY = y;
}

// Sets the visible size of the graph in pixels
void Graph::SetRenderSize(int width, int height) {
	renderWidth = width;
	renderHeight = height;
}

void Graph::SetRenderTimeRange(int range) {
	renderTimeRange = range;
}

// Sets the colour of the data line
void Graph::SetLineColour(int r, int g, int b) {
	clrR = r; clrG = g; clrB = b;
}

void Graph::SetTitleLabel(const char* title) {
	this->title = title;
}

void Graph::SetDoAutoScale(bool doAutoScale) {
	this->doAutoScale = doAutoScale;
}

void Graph::AddBar(const struct GraphBar& bar) {
	bars.Append(bar);
}

void Graph::UpdateAutoScale() {
	if (doAutoScale) {
		// Find range of values
		int minValue = (~0u)>>1, maxValue = (~0u>>1)+1;

		for (auto value : values) {
			if (value < minValue) {
				minValue = value;
			} else if (value > maxValue) {
				maxValue = value;
			}
		}

		// Update the value range
		renderMin = minValue;
		renderMax = maxValue;

		// Avoid a range of 0!
		if (renderMin == renderMax) {
			renderMax = renderMin + 1;
		}
	}
}

// Renders the graph onto the game surface at a given position
void Graph::Render() const {
	// Draw title
	game.RenderText(title, renderX, renderY, RenderScreen::Debug);
	
	// Draw border
	SDL_Rect border;

	border.x = renderX;
	border.y = renderY;
	border.w = renderWidth;
	border.h = renderHeight;

	SDL_SetRenderDrawColor(game.GetRenderer(RenderScreen::Debug), 0, 0, 0, 255);
	SDL_RenderDrawRect(game.GetRenderer(RenderScreen::Debug), &border);

	// Draw graph
	SDL_SetRenderDrawColor(game.GetRenderer(RenderScreen::Debug), clrR, clrG, clrB, 0);

	int numDrawnPoints = 0;
	SDL_Point points[maxNumValues];
	uint32 currentTime = game.GetFrameTime();

	for (int point = 0; point < maxNumValues; ++point) {
		if (valueTimestamps[point] + renderTimeRange < currentTime) {
			continue;
		}

		points[numDrawnPoints].x = renderX + (renderTimeRange - (currentTime - valueTimestamps[point])) * renderWidth / renderTimeRange;
		points[numDrawnPoints].y = renderY + renderHeight - ((values[point] - renderMin) * renderHeight / (renderMax - renderMin));

		++numDrawnPoints;
	}


	SDL_RenderDrawLines(game.GetRenderer(RenderScreen::Debug), points, numDrawnPoints);

	// Draw bars
	for (const GraphBar& bar : bars) {
		int centreX = renderX + (renderTimeRange - (currentTime - bar.time)) * renderWidth / renderTimeRange;
		int centreY = renderY + renderHeight - ((bar.value - renderMin) * renderHeight / (renderMax - renderMin));

		if (bar.orientation == GraphBar::Horizontal) {
			SDL_RenderDrawLine(game.GetRenderer(RenderScreen::Debug), centreX - bar.length, centreY, centreX + bar.length, centreY);
		} else {
			SDL_RenderDrawLine(game.GetRenderer(RenderScreen::Debug), centreX, centreY - bar.length, centreX, centreY + bar.length);
		}
	}
}