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
	
public:
	void UpdateArduinoInput();
	void UpdateKeyboardInput();

public:
	void UpdateDebugGraphs();

private:
	void RenderDebugGraphs();
	void RenderDebugVectors();
	void RenderDebugText();

public:
	// Acceleration data
	struct AccelStamp {
		AccelStamp() = default;
		AccelStamp(const Vec3& direction_, const Vec3& baseDirection_, uint32 timestamp_) : direction(direction_), baseDirection(baseDirection_), timestamp(timestamp_) {};

		// Direction transformed by the default known down vector
		Vec3 direction;

		// Untransformed raw direction
		Vec3 baseDirection;

		// Time that this accelstamp was taken
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

	// Resets the accelerometer's down, forward and right vectors according to the current gravity force
	void ResetAccelLocalVectors();

	// Gets the total number of accel stamps recorded historically
	int GetNumRecordedAccels() const;

public:
	// Gets the current bend angle of the flex sensor, in degrees
	float32 GetFlexAngle() const;

	// Sets the believed flex angle for the hand to be closed
	void SetHandClosedAngle(float32 flexAngle);

	// Sets the believed flex angle for the hand to be open
	void SetHandOpenAngle(float32 max);

	// Guesses the hand's open-ness, using the known min and max angles, in a range of 0 (closed) to 1 (fully open).
	// 'Error' is a scalar multiplier of the total range that reflects the leeway
	float32 GetHandOpenness(float32 error = 0.2f) const;

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

public:
	// Nvm let's try this boiz
	struct BounceInfo {
		// Number of bounces occurred in this time frame
		int32 numBounces;

		// Average bounces per second
		float32 averageBounceHz;

		// Average bounce amplitude (in m/s/s)
		float32 averageBounceAmplitude;

		// Max bounce amplitude (in m/s/s)
		float32 maxBounceAmplitude;

		// The average force (presumably of gravity) across the centre of each bounce
		Vec3 averageCentralForce;

		// The average central force recorded in the last bounce
		Vec3 lastCentralForce;
	};

	// Calculates bounce info across a span of time, relative to the current time, and returns the result
	BounceInfo CalculateBounceInfo(uint32 timeStart, uint32 timeEnd);

private:
	StaticCircularArray<AccelStamp, maxNumAccelStamps> accelStamps;
	int32 numAccelsRecordedTotal = 0;

	bool8 gestureDown[NumGestureCodes] = {0}; // Whether a gesture is being done

	// Angle, in degrees, of the flex sensor
	float32 flexAngle;

	// Flex angles for whether the hand's open or closed
	float32 handClosedFlexAngle;
	float32 handOpenFlexAngle;

	// Accelerometer vectors for internal transformation
	Vec3 accelRight = Vec3(1.0f, 1.0f, 0.0f);
	Vec3 accelForward = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 accelDown = Vec3(0.0f, 0.0f, 1.0f);

	// Debug stuff
	bool8 debugMode = false;

	Array<Graph> debugGraphs;

	// Debug keyboard control
	bool8 isKeyboardEnabled;

	// Raw incoming data stream from the arduino
	uint8 arduinoInStream[512];
};