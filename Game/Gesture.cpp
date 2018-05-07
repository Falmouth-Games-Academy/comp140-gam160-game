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
	isKeyboardEnabled = false;
}

void GestureManager::Update() {
	// Update graphs
	int readResult = 0;
	Serial* serial = game.GetSerialStream();

	// Read data from the serial port
	if (serial->IsConnected() && !isKeyboardEnabled) {
		// Use arduino controls
		UpdateArduinoInput();
	} else {
		// Or use keyboard controls instead
		UpdateKeyboardInput();
	}

	// Toggle debug mode
	if (game.CheckDebugPassword("graph")) {
		debugMode = !debugMode;
	}

	// Toggle keyboard mode
	if (game.CheckDebugPassword("kb")) {
		isKeyboardEnabled = !isKeyboardEnabled;
	}

	// Update debug graphs
	UpdateDebugGraphs();
}

void GestureManager::Render() {
	if (debugMode) {
		// Render debug information and objects
		RenderDebugGraphs();
		RenderDebugText();
		RenderDebugVectors();
	}
}

void GestureManager::UpdateArduinoInput() {
	// Get the serial stream
	Serial* serial = game.GetSerialStream();

	if (!serial->IsConnected()) {
		return;
	}

	// Collect bytes into the circular data array
	int numPendingBytes = 0;
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
		flexAngle = (arduinoIn->flexValue - 570) * 90.0f / (730 - 570);

		// Append the accel vector to the movement history
		Vec3 acceleration((float)arduinoIn->accelX, (float)arduinoIn->accelY, (float)arduinoIn->accelZ);
		Vec3 transformedAcceleration = Vec3(Vec3::Dot(acceleration, accelRight), Vec3::Dot(acceleration, accelForward), Vec3::Dot(acceleration, accelDown));

		accelStamps.Append(AccelStamp(transformedAcceleration, acceleration, game.GetFrameTimeMs()));
		++numAccelsRecordedTotal;
	}
}

void GestureManager::UpdateKeyboardInput() {
	// Use debug keyboard controls in place of the arduino
	Vec3 nextForce = Vec3(0.0f, 0.0f, 9800.0f);

	// Bounce
	if (game.GetInput().IsKeyDown(SDLK_UP)) {
		nextForce = Vec3(0.0f, 0.0f, 18000.0f);
	} else if (game.GetInput().IsKeyDown(SDLK_DOWN)) {
		nextForce = Vec3(0.0f, 0.0f, 2000.0f);
	}

	// AutoBounce
	const float bounceHz = 5.0f, bounceAmp = 8000.0f;
	float bouncePosition = sin(Math::circleclamp(2.0f * Math::pi * game.GetFrameTime() * bounceHz, 2.0f * Math::pi)) * bounceAmp;

	if (game.GetInput().IsKeyDown(SDLK_RIGHT)) {
		Vec2 tiltedDownVector = Vec2(0.0f, 9800.0f).Rotated(30.0f);

		nextForce = Vec3(0.0f, tiltedDownVector.x, tiltedDownVector.y) + Vec3(0.0f, 0.0f, bouncePosition);
	} else if (game.GetInput().IsKeyDown(SDLK_LEFT)) {
		nextForce = Vec3(0.0f, 0.0f, 9800.0f) + Vec3(0.0f, 0.0f, bouncePosition);
	}

	// Mouse tilt controls
	if (game.GetInput().IsMouseDown(InputManager::LeftButton)) {
		Vec3 playerScreenPosition = game.GetCamera().WorldToScreen(game.GetPlayer().GetPosition());

		nextForce = Vec3(0.0f, (game.GetInput().GetMousePosition().x - playerScreenPosition.x) * 30, (game.GetInput().GetMousePosition().y - playerScreenPosition.y) * 30);
	}

	// Append the force
	accelStamps.Append(AccelStamp(nextForce, nextForce, game.GetFrameTimeMs()));

	// Simulate flex sensor
	flexAngle = Math::clamp(Math::round(flexAngle + (float)game.GetInput().GetMouseScroll() * 10.0f, 10.0f), 0.0f, 180.0f);
}

void GestureManager::UpdateDebugGraphs() {
	Vec3 accel = accelStamps[-1].direction;

	// Update the debug graphs
	debugGraphs[0].PushValue(accel.z);
	debugGraphs[2].PushValue(accel.z);

	// Append averaged out value to the other debug graph
	const int avgSpan = 500;
	debugGraphs[1].PushValue(GetAverageAccel(avgSpan, 0).z);

	// Update even more averaged graph I guess??
	debugGraphs[3].PushValue((int)GetAverageAccel(1000, 0).z);
	debugGraphs[3].SetRenderValueRange(debugGraphs[2].GetRenderValueRange()); // average value needs to be in the same range as the graph below it!
}

void GestureManager::RenderDebugGraphs() {
	// Render the debug data graphs
	for (Graph& graph : debugGraphs) {
		graph.Render();
	}
}

void GestureManager::RenderDebugVectors() {
	float vectorScale = 0.01f;
	Vec3 average = GetAverageAccel(25, 0);
	SDL_Renderer* renderer = game.GetSDLRenderer(RenderScreen::Debug);
	const int halfSize = 150;
	SDL_Rect box = {300, 300, halfSize * 2, halfSize * 2};

	// Draw vector box
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &box);

	// Draw the XZ vector's historical position
	const uint32 historicalMs = 1000, historicalDrawIntervalMs = 25;
	SDL_Point points[historicalMs / historicalDrawIntervalMs];

	for (uint32 i = game.GetFrameTimeMs() % historicalDrawIntervalMs; i < historicalMs; i += historicalDrawIntervalMs) {
		Vec3 force = game.GetGesture().GetAverageAccel(i + historicalDrawIntervalMs, i);

		points[i / historicalDrawIntervalMs].x = box.x + halfSize + (int)(force.y * vectorScale);
		points[i / historicalDrawIntervalMs].y = box.y + halfSize + (int)(force.z * vectorScale);
	}

	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_RenderDrawLines(renderer, points, historicalMs / historicalDrawIntervalMs);
	
	// Draw the XZ vector
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
	SDL_RenderDrawLine(renderer, box.x + halfSize, box.y + halfSize, box.x + halfSize + (int)(average.y * vectorScale), box.y + halfSize + (int)(average.z * vectorScale));

	// Draw the expected rotation of the hand during a bounce
	GestureManager::BounceInfo bounceInfo = CalculateBounceInfo(300, 0);

	float targetSpeed = 0.0f;
	float acceleration = 0.0f;

	float bounceSpeed = bounceInfo.averageBounceHz * bounceInfo.averageBounceAmplitude;

	if (bounceInfo.numBounces > 0) {
		float rotation = Vec2::Direction(Vec2(0.0f, 0.0f), Vec2(-bounceInfo.lastCentralForce.y, -bounceInfo.lastCentralForce.z)) * Math::degs - 35.0f;

		SDL_RenderDrawLine(renderer, box.x + halfSize, box.y + halfSize, box.x + halfSize + (int)(-bounceInfo.lastCentralForce.y * vectorScale), box.y + halfSize + (int)(-bounceInfo.lastCentralForce.z * vectorScale));
	}

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
	strings.DrawString(StaticString<60>::FromFormat("AllAxes force: %.3f m/s/s", accelStamps[-1].direction.Length()));
	strings.DrawString(StaticString<80>::FromFormat("Total accelstamps received: %i", numAccelsRecordedTotal));
	strings.DrawString(StaticString<80>::FromFormat("Flex angle: %.2f\n", flexAngle));

	strings.Render();
}

const StaticCircularArray<GestureManager::AccelStamp, GestureManager::maxNumAccelStamps>& GestureManager::GetAccelHistory() const {
	return accelStamps;
}

Vec3 GestureManager::GetAccelAtTime(uint32 numMsAgo) const {
	uint32 targetTimestamp = game.GetFrameTimeMs() - numMsAgo;

	for (int i = 0; i > -maxNumAccelStamps + 1; --i) {
		if (accelStamps[i].timestamp <= targetTimestamp && accelStamps[i + 1].timestamp >= targetTimestamp) {
			float timeGap = (float)(accelStamps[i + 1].timestamp - accelStamps[i].timestamp);
			float higherFactor;
			
			if (timeGap != 0.0f) {
				higherFactor = (float)(targetTimestamp - accelStamps[i].timestamp) / timeGap;
			} else {
				// Don't divide by 0
				higherFactor = 1.0f;
			}

			// Return linearly interpolated value
			return accelStamps[i].direction;//Vec3::Lerp(accelHistory[i].direction, accelHistory[i + 1].direction, higherFactor);
		}
	}

	// None found
	return Vec3(0.0f, 0.0f, 0.0f);
}

Vec3 GestureManager::GetAverageAccel(uint32 relativeTimeStart, uint32 relativeTimeEnd) const {
	Vec3 average(0.0f, 0.0f, 0.0f);
	uint32 absoluteTimeStart = game.GetFrameTimeMs() - relativeTimeStart, absoluteTimeEnd = game.GetFrameTimeMs() - relativeTimeEnd;
	int numCollectedPoints = 0;
	
	// Alert programmer if they were stupid
	// I'm looking at you, Louis
	assert(relativeTimeStart > relativeTimeEnd);

	// Sum the vectors
	int lastHigherIndex = -1;
	for (int i = -1; i >= -accelStamps.GetNum(); --i) {
		const AccelStamp& stamp = accelStamps[i];

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
	uint32 absoluteTimeStart = game.GetFrameTimeMs() - relativeTimeStart, absoluteTimeEnd = game.GetFrameTimeMs() - relativeTimeEnd;
	MinMax<Vec3> minMax(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));

	// Alert programmer if they made copypasta
	// I'm looking at you, Louis
	assert(relativeTimeStart > relativeTimeEnd);

	// Take the minimum and maximum
	for (int i = -1; i >= -accelStamps.GetNum(); --i) {
		const Vec3& accel = accelStamps[i].direction;

		if (accelStamps[i].timestamp > absoluteTimeEnd) {
			continue;
		} else if (accelStamps[i].timestamp < absoluteTimeStart) {
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

void GestureManager::ResetAccelLocalVectors() {
	accelDown = accelStamps[-1].baseDirection.Normalised();
	accelForward = Vec3::Cross(Vec3(1.0f, 0.0f, 0.0f), accelDown).Normalised() * -1.0f;
	accelRight = Vec3::Cross(accelDown, accelForward);
}

int GestureManager::GetNumRecordedAccels() const {
	return numAccelsRecordedTotal;
}

float32 GestureManager::GetFlexAngle() const {
	return flexAngle;
}

void GestureManager::SetHandClosedAngle(float32 min) {
	handClosedFlexAngle = min;
}

void GestureManager::SetHandOpenAngle(float32 max) {
	handOpenFlexAngle = max;
}

float32 GestureManager::GetHandOpenness(float32 error) const {
	float absoluteError = (handOpenFlexAngle - handClosedFlexAngle) * error;

	return Math::clamp(Math::lerpfloat(flexAngle, MinMax<float>(handClosedFlexAngle + absoluteError, handOpenFlexAngle - absoluteError), MinMax<float>(0.0f, 1.0f)), 0.0f, 1.0f);
}

GestureManager::BounceInfo GestureManager::CalculateBounceInfo(uint32 relativeTimeStart, uint32 relativeTimeEnd) {
	uint32 absoluteTimeStart = game.GetFrameTimeMs() - relativeTimeStart, absoluteTimeEnd = game.GetFrameTimeMs() - relativeTimeEnd;
	
	const float bounceDistanceThreshold = 500.0f; // The distance from the peak point until a bounce is registered (should this be a percentage?)
	int currentDirection = 1;
	float32 currentExtent = -100000.0f;

	uint32 lastBounceTime;
	float32 lastBounceExtent;
	Vec3 lastBouncePosition;
	uint32 totalBounceTimeDifferences = 0;
	float32 totalBounceAmplitudes = 0.0f;
	Vec3 totalBouncePositions = Vec3(0.0f, 0.0f, 0.0f);

	// Setup bounce info
	BounceInfo info;
	info.numBounces = 0;
	info.maxBounceAmplitude = 0.0f;

	// Iterate through the history to find a bouncing pattern
	for (int i = -accelStamps.GetNum(); i < 0; ++i) {
		// Only look through the given time range
		if (accelStamps[i].timestamp < absoluteTimeStart) {
			continue;
		} else if (accelStamps[i].timestamp > absoluteTimeEnd) {
			continue;
		}

		// Estimate the current bounce level
		float currentForce = accelStamps[i].baseDirection.z;// Vec2::Distance(Vec2(0.0f, 0.0f), Vec2(accel.z, accel.y));

		// Detect whether a bounce is occurring
		// Bounces are recorded through a change in direction that goes below the peak by a certain threshold (bounceDistanceThreshold)
		bool hasBounced = false;
		if (currentDirection == 1) {
			if (currentForce > currentExtent) {
				currentExtent = currentForce;
			} else if (currentForce < currentExtent - bounceDistanceThreshold) {
				hasBounced = true;
			}
		} else if (currentDirection == -1) {
			if (currentForce  < currentExtent) {
				currentExtent = currentForce;
			} else if (currentForce > currentExtent + bounceDistanceThreshold) {
				hasBounced = true;
			}
		}

		// If this was found to be a bounce, register its info
		if (hasBounced) {
			const Vec3& transformedAccel = accelStamps[i].direction;

			if (info.numBounces > 0) {
				totalBounceTimeDifferences += accelStamps[i].timestamp - lastBounceTime;
				info.lastCentralForce = (transformedAccel + lastBouncePosition) * 0.5f;
				totalBouncePositions += info.lastCentralForce;

				float currentAmplitude = abs(currentExtent - lastBounceExtent);
				totalBounceAmplitudes += currentAmplitude;

				if (currentAmplitude > info.maxBounceAmplitude) {
					info.maxBounceAmplitude = currentAmplitude;
				}
			}

			lastBounceTime = accelStamps[i].timestamp;
			lastBounceExtent = currentExtent;
			lastBouncePosition = transformedAccel;

			currentDirection = -currentDirection;
			currentExtent = 100000.0f * -currentDirection;
			++info.numBounces;
		}
	}

	// Update BounceInfo bounce was info
	if (info.numBounces > 1) {
		info.averageBounceHz = 1000.0f / (float)(totalBounceTimeDifferences / (info.numBounces - 1));
		info.averageBounceAmplitude = totalBounceAmplitudes / (float)(info.numBounces - 1);
		info.averageCentralForce = totalBouncePositions / (float)(info.numBounces - 1);
	} else {
		info.averageBounceHz = 0.0f;
		info.averageBounceAmplitude = 0.0f;
		info.averageCentralForce = Vec3(0.0f, 0.0f, 0.0f);
	}

	// Done
	return info;
}
