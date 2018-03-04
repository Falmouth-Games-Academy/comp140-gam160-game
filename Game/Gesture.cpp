#include "Game.h"
#include "ArduinoIO.h"

#include <cassert>

GestureManager::GestureManager() {
	// Create debug graphs
	struct GraphInitData {const char* title; uint32 colour; int x, y, width, height; uint32 range;};
	const uint32 graphHeights = 80;

	for (const GraphInitData& data : std::initializer_list<GraphInitData>{
		{"Z Accel", 0x000000FF, 0, 0, 600, graphHeights, 1000}, {"", 0x0000FF00, 0, graphHeights, 600, graphHeights, 1000}, 
		{"Small time window", 0x00FF0000, 0, graphHeights * 2, 400, graphHeights, 200}, {"", 0x00000000, 0, graphHeights * 2, 400, graphHeights, 200}})
	{
		Graph& newGraph = debugGraphs.Append();

		newGraph.SetTitleLabel(data.title);
		newGraph.SetLineColour(data.colour>>16&255, data.colour>>8&255, data.colour&255);
		newGraph.SetRenderPosition(data.x, data.y);
		newGraph.SetRenderSize(data.width, data.height);
		newGraph.SetRenderValueRange({-11000, 8000});
		newGraph.SetRenderTimeRange(data.range);
		newGraph.SetDoAutoScale(true);
	}

	// Set debug keyboard value
	isKeyboardEnabled = true; // ...to true cuz yush

}

void GestureManager::Update() {
	// Update graphs
	int readResult = 0;
	Serial* serial = game.GetSerialStream();

	// Read data from the serial port
	if (serial->IsConnected()) {
		int numPendingBytes = 0;

		// Collect bytes into the circular data array
		do {
			numPendingBytes = serial->GetNumPendingBytes();

			if (numPendingBytes > 0) {
				if (numPendingBytes > 256) {
					numPendingBytes = 256;
				}

				for (int i = numPendingBytes; i < 256; ++i) {
					arduinoInStream[i - numPendingBytes] = arduinoInStream[i];
				}

				serial->ReadData((char*)&arduinoInStream[256 - numPendingBytes], numPendingBytes);
			}
		} while (numPendingBytes >= sizeof (ArduinoToPCData));

		// Find the latest Arduino IO info by checking alignment
		// Find the latest 0xDEAD (head) and 0xD00D (tail) data markers
		ArduinoToPCData* arduinoIn = nullptr;

		for (ArduinoToPCData* searchData = (ArduinoToPCData*)&arduinoInStream[sizeof (arduinoInStream) - sizeof (ArduinoToPCData)];
							searchData >= (ArduinoToPCData*)arduinoInStream;
							searchData = (ArduinoToPCData*)((uintptr)searchData - 1)) {
			if (searchData->startMarker == 0xADDE && searchData->endMarker == 0x0DD0) {
				arduinoIn = searchData;
				break;
			}
		}

		// If the data markers were found...
		if (arduinoIn) {
			// Update the flex angle
			flexAngle = arduinoIn->flexValue;

			// Append the accel vector to the movement history
			accelHistory.Append(AccelStamp(Vec3((float)arduinoIn->accelX, (float)arduinoIn->accelY, (float)arduinoIn->accelZ), game.GetFrameTime()));
			++numAccelsRecordedTotal;

			// Update the debug graphs
			debugGraphs[0].PushValue(arduinoIn->accelZ);
			debugGraphs[2].PushValue(arduinoIn->accelZ);

			// Append averaged out value to the other debug graph
			const uint32 avgSpan = 500; // in ms
			const uint32 targetTime = game.GetFrameTime() - avgSpan;
			float totalValueSum = 0.0f;
			uint32 numFound = 0;

			for (int i = -1; i >= -accelHistory.GetNum(); --i) {
				if (accelHistory[i].timestamp < targetTime)
					break;

				totalValueSum += accelHistory[i].direction.z;
				++numFound;
			}

			if (numFound) {
				debugGraphs[1].PushValue((int)totalValueSum / numFound);
			}
		}
	} else if (isKeyboardEnabled) {
		// Use debug keyboard controls in place of the arduino
		if (game.GetInput().IsKeyDown(SDLK_UP)) {
			accelHistory.Append(AccelStamp(Vec3(0.0f, 0.0f, 12000.0f), game.GetFrameTime()));
		} else if (game.GetInput().IsKeyDown(SDLK_DOWN)) {
			accelHistory.Append(AccelStamp(Vec3(0.0f, 0.0f, 6000.0f), game.GetFrameTime()));
		} else if (game.GetInput().IsKeyDown(SDLK_k)) {
			// Mouse tilt controls then?
			Vec3 playerScreenPosition = game.GetCamera().WorldToScreen(game.GetPlayer().GetPosition());
			accelHistory.Append(AccelStamp(Vec3((game.GetInput().GetMousePosition().x - playerScreenPosition.x) * 10, 0.0f, (game.GetInput().GetMousePosition().y - playerScreenPosition.y) * 10), game.GetFrameTime()));
		} else {
			// Advanced gravity simulation to the nearest whatevs units of meh
			accelHistory.Append(AccelStamp(Vec3(0.0f, 0.0f, 9000.0f), game.GetFrameTime()));
		}
	}

	// Update average graph value
	debugGraphs[3].PushValue((int)GetAverageAccel(1000, 0).z);
	debugGraphs[3].SetRenderValueRange(debugGraphs[2].GetRenderValueRange()); // average value needs to be in the same range as t he graph below it!

	// Toggle debug mode
	if (game.GetInput().IsKeyBooped(SDLK_d)) {
		debugMode = !debugMode;
	}
}

void GestureManager::Render() {
	if (debugMode) {
		RenderDebugGraphs();
		RenderDebugText();
		RenderDebugVectors();
	}
}

void GestureManager::RenderDebugGraphs() {
	// Render the debug data graphs
	for (Graph& graph : debugGraphs) {
		graph.Render();
	}
}

void GestureManager::RenderDebugVectors() {
	float vectorScale = 0.01f;
	Vec3 average = GetAverageAccel(50, 0);
	SDL_Renderer* renderer = game.GetRenderer(RenderScreen::Debug);
	const int halfSize = 150;
	SDL_Rect box = {300, 300, halfSize * 2, halfSize * 2};

	// Draw vector box
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &box);

	// Draw the XZ vector's historical position
	const uint32 historicalMs = 1000, historicalDrawIntervalMs = 25;
	SDL_Point points[historicalMs / historicalDrawIntervalMs];

	for (uint32 i = game.GetFrameTime() % historicalDrawIntervalMs; i < historicalMs; i += historicalDrawIntervalMs) {
		Vec3 force = game.GetGesture().GetAverageAccel(i + historicalDrawIntervalMs, i);

		points[i / historicalDrawIntervalMs].x = box.x + halfSize + (int)(force.x * vectorScale);
		points[i / historicalDrawIntervalMs].y = box.y + halfSize + (int)(force.z * vectorScale);
	}

	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_RenderDrawLines(renderer, points, historicalMs / historicalDrawIntervalMs);
	
	// Draw the XZ vector
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
	SDL_RenderDrawLine(renderer, box.x + halfSize, box.y + halfSize, box.x + halfSize + (int)(average.x * vectorScale), box.y + halfSize + (int)(average.z * vectorScale));

	// Draw the XZ vector compensating for changes in length
	MinMax<Vec3> minMax = GetMinMaxAccel(50, 0);
	Vec3 test = minMax.min + minMax.max / 2;

	SDL_RenderDrawLine(renderer, box.x + halfSize, box.y + halfSize, box.x + halfSize + (int)(test.x * vectorScale), box.y + halfSize + (int)(test.z * vectorScale));
}

void GestureManager::RenderDebugText() {
	// Render debug text
	DebugStringBox strings(RenderScreen::Debug, 0, 300, 300, 300);
	const float restingValue = 9750.5f;//9640.0f;
	const uint32 timeWindowMs = 100;
	MinMax<Vec3> minMax = GetMinMaxAccel(timeWindowMs, 0);

	strings.DrawString(StaticString<60>::FromFormat("Window length: %i ms", timeWindowMs));
	strings.DrawString(StaticString<60>::FromFormat("Average value: %.3f", GetAverageAccel(timeWindowMs, 0).z));
	strings.DrawString(StaticString<100>::FromFormat("Min: %.3f Max: %.3f Range: %.3f", minMax.min.z, minMax.max.z, minMax.GetRange() / 1000.0f));
	strings.DrawString(StaticString<60>::FromFormat("Average from resting: %.2f", GetAverageAccel(timeWindowMs, 0).z - restingValue));
	//strings.DrawString(StaticString<60>::FromFormat("Current guesstimated speed: %.2f", currentSpeed));
	strings.DrawString(StaticString<60>::FromFormat("AllAxes force: %.3f m/s/s", accelHistory[-1].direction.Length()));
	strings.DrawString(StaticString<80>::FromFormat("Total accelstamps received: %i", numAccelsRecordedTotal));
	strings.DrawString(StaticString<80>::FromFormat("Flex angle: %.2f\n", flexAngle));
}

const StaticCircularArray<GestureManager::AccelStamp, GestureManager::maxNumAccelStamps>& GestureManager::GetAccelHistory() const {
	return accelHistory;
}

Vec3 GestureManager::GetAccelAtTime(uint32 numMsAgo) const {
	uint32 targetTimestamp = game.GetFrameTime() - numMsAgo;

	for (int i = 0; i > -maxNumAccelStamps + 1; --i) {
		if (accelHistory[i].timestamp <= targetTimestamp && accelHistory[i + 1].timestamp >= targetTimestamp) {
			float timeGap = (float)(accelHistory[i + 1].timestamp - accelHistory[i].timestamp);
			float higherFactor;
			
			if (timeGap != 0.0f) {
				higherFactor = (float)(targetTimestamp - accelHistory[i].timestamp) / timeGap;
			} else {
				// Don't divide by 0
				higherFactor = 1.0f;
			}

			// Return linearly interpolated value
			return accelHistory[i].direction;//Vec3::Lerp(accelHistory[i].direction, accelHistory[i + 1].direction, higherFactor);
		}
	}

	// None found
	return Vec3(0.0f, 0.0f, 0.0f);
}

Vec3 GestureManager::GetAverageAccel(uint32 relativeTimeStart, uint32 relativeTimeEnd) const {
	Vec3 average(0.0f, 0.0f, 0.0f);
	uint32 absoluteTimeStart = game.GetFrameTime() - relativeTimeStart, absoluteTimeEnd = game.GetFrameTime() - relativeTimeEnd;
	int numCollectedPoints = 0;
	
	// Alert programmer if they were stupid
	// I'm looking at you, Louis
	assert(relativeTimeStart > relativeTimeEnd);

	// Sum the vectors
	int lastHigherIndex = -1;
	for (int i = -1; i >= -accelHistory.GetNum(); --i) {
		const AccelStamp& stamp = accelHistory[i];

		if (stamp.timestamp >= absoluteTimeStart && stamp.timestamp <= absoluteTimeEnd) {
			average += stamp.direction;
			++numCollectedPoints;
		}

		if (stamp.timestamp > absoluteTimeStart) {
			lastHigherIndex = i;
		}
	}

	// Return the average direction without dividing by 0
	if (numCollectedPoints > 0) {
		return average / (float)numCollectedPoints;
	} /*else if (lastHigherIndex != -1) {
		return accelHistory[lastHigherIndex].direction;
	} */else {
		// If no average was found, maybe try getting the interpolated value
		return GetAccelAtTime(relativeTimeStart + relativeTimeEnd / 2);
	}
}

MinMax<Vec3> GestureManager::GetMinMaxAccel(uint32 relativeTimeStart, uint32 relativeTimeEnd) const {
	bool foundAny = false;
	uint32 absoluteTimeStart = game.GetFrameTime() - relativeTimeStart, absoluteTimeEnd = game.GetFrameTime() - relativeTimeEnd;
	MinMax<Vec3> minMax(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));

	// Alert programmer if they made copypasta
	// I'm looking at you, Louis
	assert(relativeTimeStart > relativeTimeEnd);

	// Take the minimum and maximum
	for (int i = -1; i >= -accelHistory.GetNum(); --i) {
		const Vec3& accel = accelHistory[i].direction;

		if (accelHistory[i].timestamp > absoluteTimeEnd) {
			continue;
		} else if (accelHistory[i].timestamp < absoluteTimeStart) {
			break;
		}

		if (!foundAny) {
			// This is the first value found so use this to initialise minMax
			minMax.min.x = minMax.max.x = accel.x;
			minMax.min.y = minMax.max.y = accel.y;
			minMax.min.z = minMax.max.z = accel.z;
			foundAny = true;
		} else {
			if (accel.x < minMax.min.x) {
				minMax.min.x = accel.x;
			}
			if (accel.y < minMax.min.y) {
				minMax.min.y = accel.y;
			}
			if (accel.z < minMax.min.z) {
				minMax.min.z = accel.z;
			}
			if (accel.x > minMax.max.x) {
				minMax.max.x = accel.x;
			}
			if (accel.y > minMax.max.y) {
				minMax.max.y = accel.y;
			}
			if (accel.z > minMax.max.z) {
				minMax.max.z = accel.z;
			}
		}
	}

	return minMax;
}

int GestureManager::GetNumRecordedAccels() const {
	return numAccelsRecordedTotal;
}

float GestureManager::GetFlexAngle() const
{
	return flexAngle;
}
