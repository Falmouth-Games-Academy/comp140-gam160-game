#pragma once
#include "Containers.h"
#include "Math.h"

class GestureManager {
private:
	static const int maxNumAccelStamps = 2000;
public:
	GestureManager();

	void Update();
	void Render();
	
private:
	void RenderDebugGraphs();
	void RenderDebugVectors();
	void RenderDebugText();

public:
	// Acceleration data
	struct AccelStamp {
		AccelStamp() = default;
		AccelStamp(const Vec3& direction_, uint32 timestamp_) : direction(direction_), timestamp(timestamp_) {};

		Vec3 direction;
		uint32 timestamp;
	};

	// Gets the accelerometer history array
	// The array can be traversed forwards from 0, or backwards from -1
	const StaticCircularArray<AccelStamp, maxNumAccelStamps>& GetAccelHistory() const;

	// Gets an interpolated acceleration value at the given time, in ms passed from now
	Vec3 GetAccelAtTime(uint32 numMsAgo) const;

	// Gets the average accelerometer value across a span of time, relative to the current time
	// 0 means now, and 100 means 100 ms ago--if your timeEnd parameter is larger than timeStart you're using this function wrong!
	Vec3 GetAverageAccel(uint32 timeStart, uint32 timeEnd) const;

	// Get the minimum and maximum accelerometer values across a span of time
	MinMax<Vec3> GetMinMaxAccel(uint32 timeStart, uint32 timeEnd) const;

	// Gets the number of accel stamps recorded historically--this may be more than the array contains
	int GetNumRecordedAccels() const;

	// Gets the current bend angle of the flex sensor, in degrees
	float GetFlexAngle() const;
public:
	// Gesture enum
	enum GestureCode {
		Jumping = 0,
		Walking,
		NumGestureCodes,
	};

	bool IsGestureDown() const;
	bool IsGestureBooped() const;

	int GetWalkRate() const;
	int GetWalkIntensity() const;

private:
	StaticCircularArray<AccelStamp, maxNumAccelStamps> accelHistory;
	int32 numAccelsRecordedTotal = 0;

	bool8 gestureDown[NumGestureCodes] = {0}; // Whether a gesture is being done

	// Angle, in degrees, of the flex sensor
	float flexAngle;

	bool8 debugMode = false;

	Array<Graph> debugGraphs;

	// Raw incoming data stream from the arduino
	uint8 arduinoInStream[512];
};