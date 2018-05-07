#pragma once

#include <cmath> // sqrt
#include "Types.h"


// Undef min and max (thanks Windows)
#ifdef min
#undef min
#undef max
#endif

// Predeclarations for Math functions
template<typename T> class MinMax;

namespace Math {
	// Constants
	const float pi = 3.141593f;

	// Conversion between degrees and radians: multiply by these to convert to the unit
	const float rads = pi / 180.0f;
	const float degs = 180.0f / pi;

	// Sign/negative/positive
	template<typename SignType>
	inline SignType getsign(SignType value) {return (value > (SignType)(0) ? (SignType)(1) : (value < (SignType)(0) ? (SignType)(-1) : (SignType)(0)));}

	// Clamping
	template<typename ClampType>
	inline ClampType clampmin(ClampType value, ClampType min) {if (value < min) {return min;} else {return value;}}
	template<typename ClampType>
	inline ClampType clampmax(ClampType value, ClampType max) {if (value > max) {return max;} else {return value;}}
	template<typename ClampType>
	inline ClampType clamp(ClampType value, ClampType min, ClampType max) {if (value > max) return max; else if (value < min) return min; else return value;}

	// Circular numbers
	// Returns shortest distance, positive or negative, between two values of a circular number, with maximum circleSize degrees
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

	// Circular clamping
	template<typename ClampType>
	inline ClampType circleclamp(ClampType value, ClampType max) { // Note: Max is exclusive
		// Basically just a templated modulo (todo: consider large ints, even though that is 99% beyond the scope of this project)
		return value - (int)(value / max) * max;
	}

	// Rounding
	template<typename RoundType>
	inline RoundType round(RoundType value, RoundType interval) {
		return (int)((value + ((float)interval * 0.5f)) / interval) * interval;
	}

	// Lerping
	// Lerp a value from an old range to a new range
	inline float lerpfloat(float value, const MinMax<float>& oldRange, const MinMax<float>& newRange);

	// Lerp a value from 0-1 to a range
	inline float lerpfloat(float value, const MinMax<float>& newRange);

	// Random number gesus
	// Returns a random floating-point number between min and max (max exclusive)
	inline float randfloat(float min, float max) {
		return ((float)rand() / (float)RAND_MAX) * (max - min) + min;
	}

	// Returns 1 or -1 randomly
	inline char randsign() {
		if (rand() & 1) {
			return 1;
		} else {
			return -1;
		}
	}
};

// 2-dimensional vector
class Vec2 {
public:
	// Constructors
	Vec2() = default;
	Vec2(float xy) : x(xy), y(xy) {};
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

	// Comparison operators overloads (compares each field)
	bool operator==(const Vec2& other) const {return x == other.x && y == other.y;}

	bool operator>=(const Vec2& other) const {return x >= other.x && y >= other.y;}
	bool operator<=(const Vec2& other) const {return x <= other.x && y <= other.y;}
	bool operator>(const Vec2& other) const {return x > other.x && y > other.y;}
	bool operator<(const Vec2& other) const {return x < other.x && y < other.y;}

	// Returns the length of the vector
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

	// Returns the dot product of two vectors
	static float Dot(const Vec2& vecA, const Vec2& vecB) {
		return vecA.x * vecB.x + vecA.y * vecB.y;
	}

	// Transformation functions
	// Normalises the vector. If it has no length, sets it to an up vector (y=-1).
	Vec2& Normalise() {float length = (float)sqrt((x * x) + (y * y)); if (length > 0.0f) {x /= length; y /= length;} else {y = -1.0f;}; return *this;}

	// Returns a rotated copy of the vector, in degrees
	Vec2 Rotated(float angle) const {float s = sin(angle * Math::rads), c = cos(angle * Math::rads); return Vec2(x * c - y * s, x * s + y * c);}

	// Returns a vecotr lerped toward vecB by factor
	Vec2 Lerped(const Vec2& vecB, float factor) const {return Vec2(vecB.x * factor + x * (1.0f - factor), vecB.y * factor + y * (1.0f - factor));};

	// Generator functions
	static Vec2 FromRotation(float angle, float magnitude = 1.0f) {
		return Vec2(sin(angle * Math::rads) * magnitude, -cos(angle * Math::rads) * magnitude);
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

	// Comparison operators overloads (compares each field)
	bool operator==(const Vec3& other) const {return x == other.x && y == other.y && z == other.z;}

	bool operator>=(const Vec3& other) const {return x >= other.x && y >= other.y && z >= other.z;}
	bool operator<=(const Vec3& other) const {return x <= other.x && y <= other.y && z <= other.z;}
	bool operator>(const Vec3& other) const {return x > other.x && y > other.y && z > other.z;}
	bool operator<(const Vec3& other) const {return x < other.x && y < other.y && z < other.z;}

	// Extremely amazing advanced vector code
	float Length() const {return (float)sqrt((x * x) + (y * y) + (z * z));}

	// Returns a vector lerped toward vecB by factor
	Vec3 Lerped(const Vec3& vecB, float factor) const {
		return Vec3(vecB.x * factor + x * (1.0f - factor), vecB.y * factor + y * (1.0f - factor), vecB.z * factor + z * (1.0f - factor));
	}

	// Returns a normalised version of this vector
	Vec3 Normalised() const {
		float length = sqrt(x * x + y * y + z * z);
		return Vec3(x / length, y / length, z / length);
	}

	// Static functions
	static Vec3 Lerp(const Vec3& vecA, const Vec3& vecB, float bFactor) {
		return Vec3(vecB.x * bFactor + vecA.x * (1.0f - bFactor), vecB.y * bFactor + vecA.y * (1.0f - bFactor), vecB.z * bFactor + vecA.z * (1.0f - bFactor));
	}

	static float Distance(const Vec3& vecA, const Vec3& vecB) {
		return (float)sqrt((vecA.x - vecB.x) * (vecA.x - vecB.x) + (vecA.y - vecB.y) * (vecA.y - vecB.y) + (vecA.z - vecB.z) * (vecA.z - vecB.z));
	}

	static Vec3 Cross(const Vec3& vecA, const Vec3& vecB) {
		return Vec3(vecA.y * vecB.z - vecA.z * vecB.y, vecA.z * vecB.x - vecA.x * vecB.z, vecA.x * vecB.y - vecA.y * vecB.x);
	}

	static float Dot(const Vec3& vecA, const Vec3& vecB) {
		return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
	}

	// Variables
	union {
		struct {
			float32 x;
			float32 y;
			float32 z;
		};

		struct {
			Vec2 xy;
			float32 _pad;
		};

		struct {
			float32 __pad;
			Vec2 yz;
		};
	};
};

// 2D Rectangle
class Rect2 {
public:
	Rect2() = default;
	Rect2(float32 x_, float32 y_, float32 width_, float32 height_) : x(x_), y(y_), width(width_), height(height_) {};
	Rect2(const Vec2& position_, const Vec2& size_) : position(position_), size(size_) {};

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

// 2D rectangle boundaries: basically a rectangle with absolute right and bottom edges
class Bounds2 {
public:
	Bounds2() = default;
	Bounds2(const Vec2& topLeft_, const Vec2& bottomRight_) : topLeft(topLeft_), bottomRight(bottomRight_) {};
	Bounds2(float32 left_, float32 top_, float32 right_, float32 bottom_) : topLeft(left_, top_), bottomRight(right_, bottom_) {};

	union {
		struct {
			float32 left, top;
		};
		Vec2 topLeft;
	};

	union {
		struct {
			float32 right, bottom;
		};
		Vec2 bottomRight;
	};
};

// 2-dimensional integer dimensions
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

// Math definitions that couldn't be done because due to incomplete class/struct types
inline float Math::lerpfloat(float value, const MinMax<float>& oldRange, const MinMax<float>& newRange) {
	return newRange.min + (value - oldRange.min) * (newRange.max - newRange.min) / (oldRange.max - oldRange.min);
}

inline float Math::lerpfloat(float value, const MinMax<float>& newRange) {
	return newRange.min + value * (newRange.max - newRange.min);
}