#include "Game.h"

#include <cassert>

GestureManager::GestureManager() {
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
}

void GestureManager::Update() {
	static uint8 incomingData[256];

	// Update graphs
	int readResult = 0;
	Serial* serial = game.GetSerialStream();

	// Read data from the serial port
	if (serial->IsConnected()) {
		int x = 0, y = 0, z = 0;
		int numPendingBytes = 0;

		// Collect bytes into the circular data array
		do {
			numPendingBytes = serial->GetNumPendingBytes();

			if (numPendingBytes > 0) {
				if (numPendingBytes > 256) {
					numPendingBytes = 256;
				}

				for (int i = numPendingBytes; i < 256; ++i) {
					incomingData[i - numPendingBytes] = incomingData[i];
				}

				serial->ReadData((char*)&incomingData[256 - numPendingBytes], numPendingBytes);
			}
		} while (numPendingBytes >= 12);

		// Find the latest 0xDEAD (head) and 0xD00D (tail) data markers
		int headByte = -1, tailByte = -1;
		for (int i = 255; i >= 0; --i) {
			if (tailByte != -1 && incomingData[i] == 0xDE && incomingData[i + 1] == 0xAD) {
				headByte = i;

				if (tailByte == headByte + 8) {
					break;
				}
			}

			if (tailByte == -1 && incomingData[i] == 0xD0 && incomingData[i + 1] == 0x0D) {
				tailByte = i;
			}
		}

		// If the data markers were found...
		if (tailByte == headByte + 8) {
			x = *(__int16*)(&incomingData[headByte + 2]);
			y = *(__int16*)(&incomingData[headByte + 4]);
			z = *(__int16*)(&incomingData[headByte + 6]);

			// Append the accel vector to the history
			accelHistory.Append(AccelStamp(Vec3((float)x, (float)y, (float)z), game.GetFrameTime()));
			++numAccelsRecordedTotal;

			// Update the debug graphs
			debugGraphs[0].PushValue(z);
			debugGraphs[2].PushValue(z);

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
				debugGraphs[1].PushValue(totalValueSum / (float)numFound);
			}
		}
	}

	// Update average graph value
	debugGraphs[3].PushValue(GetAverageAccel(1000, 0).z);
	debugGraphs[3].SetRenderValueRange(debugGraphs[2].GetRenderValueRange()); // average value needs to be in the same range as t he graph below it!

	// Test bars
	if (game.GetInput().IsInputBooped(SDLK_SPACE)) {
		debugGraphs[0].AddBar(Graph::GraphBar(Graph::GraphBar::Horizontal, 400, 0, 20));
	}
}

static uint32 timeWindowMs = 100;

void GestureManager::Render() {
	RenderDebugGraphs();
	RenderDebugText();
	RenderDebugVectors();

	// Collect historical statistical data
	static float currentSpeed = 0.0f;
	static uint32 lastNumAccelsOnSpeedRecord = 0;
	float movementThreshold = 3000.0f;
	int numMovementsAboveThreshold = 0;
	lastNumAccelsOnSpeedRecord = numAccelsRecordedTotal;

	// Try and detect average frequency of movement
	/*uint32 lastBelowAverage = 0;
	uint32 lastAboveAverage = 0;
	float averageDeadzone = 1000.0f;
	float averageValue = GetAverageAccel(100, 0).direction.z;

	for (int i = -accelHistory.GetNum(); i < 0; ++i) {
		AccelStamp& accel = accelHistory[i];

		if (accel.direction.z > movementThreshold) {
			if (lastBelowAverage != 0) {
				// found etc

			}
		}

		if (accel.direction.z < averageValue + averageDeadzone) {
			lastBelowAverage = accel.timestamp;
		}
		
		if (accel.direction.z > averageValue - averageDeadzone) {
			lastAboveAverage = accel.timestamp;
		}
	}

	// Reset speed after resting for a while
	static uint32 lastDisturbTime = 0;

	if (game.GetInput().IsInputBooped(SDLK_SPACE))
		lastDisturbTime = game.GetFrameTime();

	if (maxValue - minValue >= 1500.0f)
		lastDisturbTime = game.GetFrameTime();

	if (game.GetFrameTime() - lastDisturbTime > 300)
		currentSpeed = 0.0f;*/

}

void GestureManager::RenderDebugGraphs() {
	// Render the debug data graphs
	for (Graph& graph : debugGraphs) {
		graph.Render();
	}
}

void GestureManager::RenderDebugVectors() {
	float vectorScale = 0.03f;
	Vec3 average = GetAverageAccel(50, 0);
	SDL_Renderer* renderer = game.GetRenderer(Game::Debug);
	const int qSize = 50;
	SDL_Rect box = {300, 300, qSize * 4, qSize * 4};

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &box);

	// XY
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderDrawLine(renderer, box.x + qSize, box.y + qSize, box.x + qSize + average.x * vectorScale, box.y + qSize + average.y * vectorScale);
}

void GestureManager::RenderDebugText() {
	// Render debug text
	DebugStringBox strings(Game::Debug, 0, 300, 300, 300);
	const float restingValue = 9750.5f;//9640.0f;
	MinMax<Vec3> minMax = GetMinMaxAccel(timeWindowMs, 0);

	strings.DrawString(StaticString<60>::FromFormat("Window length: %i ms", timeWindowMs));
	strings.DrawString(StaticString<60>::FromFormat("Average value: %.3f", GetAverageAccel(timeWindowMs, 0).z));
	strings.DrawString(StaticString<100>::FromFormat("Min: %.3f Max: %.3f Range: %.3f", minMax.min.z, minMax.max.z, minMax.GetRange() / 1000.0f));
	strings.DrawString(StaticString<60>::FromFormat("Average from resting: %.2f", GetAverageAccel(timeWindowMs, 0).z - restingValue));
	//strings.DrawString(StaticString<60>::FromFormat("Current guesstimated speed: %.2f", currentSpeed));
	strings.DrawString(StaticString<60>::FromFormat("AllAxes force: %.3f m/s/s", accelHistory[-1].direction.Length()));
	strings.DrawString(StaticString<80>::FromFormat("Total accelstamps received: %i", numAccelsRecordedTotal));
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
			if (accel.x < minMax.min.x) {
				minMax.min.x = accel.x;
			}
			if (accel.y < minMax.min.y) {
				minMax.min.y = accel.y;
			}
			if (accel.z < minMax.min.z) {
				minMax.min.z = accel.z;
			}
		}
	}

	return minMax;
}

int GestureManager::GetNumRecordedAccels() const {
	return numAccelsRecordedTotal;
}