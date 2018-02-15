#pragma once
#include "Containers.h"
#include "String.h"
#include "Math.h"

// Data graphs for visualising Arduino input streams
class Graph {
public:
	// Renders the graph onto the game surface at a given position
	void Render() const;

	// Pushes a new value onto the end of the graph
	void PushValue(int value);

	// Pushes a new value at a time
	void PushValueAtTime(int value, uint32 timestamp);

	// Sets the value scale for rendering
	void SetRenderValueRange(MinMax<int32> scale);

	// Returns the value scale for rendering
	MinMax<int32> GetRenderValueRange() const;

	// Sets the position of the graph on the screen, in pixels
	void SetRenderPosition(int x, int y);

	// Sets the visible size of the graph in pixels
	void SetRenderSize(int width, int height);

	// Sets the rendered time range
	void SetRenderTimeRange(int range);

	// Sets the colour of the data line
	void SetLineColour(int r, int g, int b);

	// Sets title label
	void SetTitleLabel(const char* title);

	// Sets whether the scale is automatically updated
	void SetDoAutoScale(bool doAutoScale);

	// Adds a new bar to the graph
	struct GraphBar {
		enum Orientation {
			Horizontal = 0,
			Vertical = 1,
		};

		// Initialiser
		GraphBar(Orientation orientation_, int32 time_, int32 value_, int32 length_ = 20) : 
			orientation(orientation_), time(time_), value(value_), length(length_) {};

		Orientation orientation; // naming convention?
		uint32 time; // Time this point was placed on (for both bars this is the centre X position)
		int32 value; // Value of this bar (for horizontal bars this is the height)
		int32 length; // Length of the bar in pixels; -1 for whole span
	};

	void AddBar(const struct GraphBar& bar);
	
private:
	void UpdateAutoScale();

private:
	static const int32 maxNumValues = 2000;

	// Line colour
	uint8 clrR = 0, clrG = 0, clrB = 0;

	// Render scaling and positioning variables, in either pixels or value units
	int32 renderMin = -13000, renderMax = 13000;
	int32 renderX = 0, renderY = 0;
	int32 renderWidth = 500, renderHeight = 150;
	int32 renderTimeRange = 2000; // in ms

	bool8 doAutoScale = false;

	// Title and other identity info
	StaticString<64> title;

	// Value storage
	int32 values[maxNumValues]{0};
	uint32 valueTimestamps[maxNumValues]{0};
	int32 numValues = 0;

	// Bars (for debugging)
	Array<GraphBar> bars;
};