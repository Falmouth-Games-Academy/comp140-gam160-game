#pragma once

#include <cmath> // sqrt
#include "Types.h"


// Undef min and max (thanks Windows)
#ifdef min
#undef min
#undef max
#endif

namespace Math {
	// Constants
	const float pi = 3.141593f;

	// Conversion between degrees and radians: multiply by these to convert to the unit
	const float rads = pi / 180.0f;
	const float degs = 180.0f / pi;

	// Clamping
	template<typename ClampType>
	inline ClampType clampmin(ClampType value, ClampType min) {if (value < min) {return min;} else {return value;}}
	template<typename ClampType>
	inline ClampType clampmax(ClampType value, ClampType max) {if (value > max) {return max;} else {return value;}}

	// Circular numbers
	// anglediff: Returns shortest distance, positive or negative, between two values of a circular number of circleSize degrees
	inline float circlediff(float from, float to, float circleSize) {
		float value = to - from;

		// todo: optimise with actual math (lazy code)
		while (value < 0.0f) {
			value += circleSize;
		}
		while (value > circleSize) {
			value -= circleSize;
		}

		if (value > circleSize * 0.5f) {
			value -= circleSize;
		}
		return value;
	}
};

// 2-dimensional vector
class Vec2 {
public:
	// Constructors
	Vec2() = default;
	Vec2(float x_, float y_) : x(x_), y(y_) {};

	// Traditional vector math functions follow
	Vec2 operator+(const Vec2& other) const {Vec2 temp; temp.x = x + other.x; temp.y = y + other.y; return temp;}
	Vec2 operator-(const Vec2& other) const {Vec2 temp; temp.x = x - other.x; temp.y = y - other.y; return temp;}
	Vec2 operator*(const Vec2& other) const {Vec2 temp; temp.x = x * other.x; temp.y = y * other.y; return temp;}
	Vec2 operator/(const Vec2& other) const {Vec2 temp; temp.x = x / other.x; temp.y = y / other.y; return temp;}
	Vec2 operator*(const float scalar) const {Vec2 temp; temp.x = x * scalar; temp.y = y * scalar; return temp;}
	Vec2 operator/(const float scalar) const {Vec2 temp; temp.x = x / scalar; temp.y = y / scalar; return temp;}

	void operator+=(const Vec2& other) {x += other.x; y += other.y;}
	void operator-=(const Vec2& other) {x -= other.x; y -= other.y;}
	void operator*=(const Vec2& other) {x *= other.x; y *= other.y;}
	void operator/=(const Vec2& other) {x /= other.x; y /= other.y;}
	void operator*=(const float scalar) {x *= scalar; y *= scalar;}
	void operator/=(const float scalar) {x /= scalar; y /= scalar;}

	float Length() const {return (float)sqrt((x * x) + (y * y));}

	// Deeper math functions
	// Returns whether this vector is between a box where min is the top-left corner and max is the bottom-right corner
	bool IsWithin(const Vec2& min, const Vec2& max) {
		return (x >= min.x && y >= min.y && x <= max.x && y <= max.y);
	}

	// Static vector functions
	// Returns distance between two points
	static float Distance(const Vec2& vecA, const Vec2& vecB) {
		return (float)sqrt((vecA.x - vecB.x) * (vecA.x - vecB.x) + (vecA.y - vecB.y) * (vecA.y - vecB.y));
	}

	// Returns direction from one point to another, in radians, going clockwise where 0 is up (0 = -y)
	static float Direction(const Vec2& vecA, const Vec2& vecB) {
		return -atan2(vecA.x - vecB.x, vecA.y - vecB.y);
	}

	// Variables
	float32 x;
	float32 y;
};

// 3-dimensional vector copypasta
class Vec3 {
public:
	// Constructors
	Vec3() = default;
	Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {};
	Vec3(const Vec2& vec, float z_ = 0.0f) : x(vec.x), y(vec.y), z(z_) {};

	// Traditional vector math functions follow
	Vec3 operator+(const Vec3& other) const {Vec3 temp; temp.x = x + other.x; temp.y = y + other.y; temp.z = z + other.z; return temp;}
	Vec3 operator-(const Vec3& other) const {Vec3 temp; temp.x = x - other.x; temp.y = y - other.y; temp.z = z - other.z; return temp;}
	Vec3 operator*(const Vec3& other) const {Vec3 temp; temp.x = x * other.x; temp.y = y * other.y; temp.z = y * other.z; return temp;}
	Vec3 operator/(const Vec3& other) const {Vec3 temp; temp.x = x / other.x; temp.y = y / other.y; temp.z = y / other.z; return temp;}
	Vec3 operator*(const float scalar) const {Vec3 temp; temp.x = x * scalar; temp.y = y * scalar; temp.z = z * scalar; return temp;}
	Vec3 operator/(const float scalar) const {Vec3 temp; temp.x = x / scalar; temp.y = y / scalar; temp.z = z / scalar; return temp;}

	void operator+=(const Vec3& other) {x += other.x; y += other.y; z += other.z;}
	void operator-=(const Vec3& other) {x -= other.x; y -= other.y; z -= other.z;}
	void operator*=(const Vec3& other) {x *= other.x; y *= other.y; z *= other.z;}
	void operator/=(const Vec3& other) {x /= other.x; y /= other.y; z /= other.z;}
	void operator*=(const float scalar) {x *= scalar; y *= scalar; z *= scalar;}
	void operator/=(const float scalar) {x /= scalar; y /= scalar; z /= scalar;}

	// Extremely amazing advanced vector code
	float Length() const {return (float)sqrt((x * x) + (y * y) + (z * z));}

	// Static functions
	static Vec3 Lerp(const Vec3& vecA, const Vec3& vecB, float bFactor) {
		return Vec3(vecB.x * bFactor + vecA.x * (1.0f - bFactor), vecB.y * bFactor + vecA.y * (1.0f - bFactor), vecB.z * bFactor + vecA.z * (1.0f - bFactor));
	};

	// Variables
	union {
		struct {
			float32 x;
			float32 y;
		};
		Vec2 xy;
	};

	float32 z;
};

// 2D Rectangle
class Rect2 {
public:
	Rect2() = default;
	Rect2(float32 _x, float32 _y) : x(_x), y(_y) {};

	union {
		struct {
			float32 x, y;
		};
		Vec2 position;
	};

	union {
		struct {
			float32 width, height;
		};
		Vec2 size;
	};
};

// 2-dimensional boundary
class Dimensions2 {
public:
	Dimensions2() = default;
	Dimensions2(int32 width_, int32 height_) : width(width_), height(height_) {};

	int32 width, height;
};

// Min/Max pair for function return values
template<typename MinMaxType>
class MinMax {
public:
	MinMax() = default;
	MinMax(const MinMaxType& min_, const MinMaxType& max_) : min(min_), max(max_) {};

	// GetRange: Returns the difference between max and min
	MinMaxType GetRange() const {return max - min;}

	MinMaxType min, max;
};