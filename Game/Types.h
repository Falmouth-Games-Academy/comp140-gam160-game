#pragma once

// Primitive data types (currently Windows-only)
typedef __int64			 int64;
typedef unsigned __int64 uint64;
typedef __int32			 int32;
typedef unsigned __int32 uint32;
typedef __int16			 int16;
typedef unsigned __int16 uint16;
typedef __int8			 int8;
typedef unsigned __int8 uint8;

// Numeric address pointer
typedef unsigned __int64 uintptr;

// Reliably-sized boolean type
typedef __int8 bool8;

// Unreliably-sized float because floats are unreliable
typedef float float32;

// Colour type
struct Colour {
	Colour() = default;
	constexpr Colour(uint8 r_, uint8 g_, uint8 b_, uint8 a_) : r(r_), g(g_), b(b_), a(a_) {};
	constexpr Colour(uint32 argb) : b(argb & 0xFF), g((argb >> 8) & 0xFF), r((argb >> 16) & 0xFF), a((argb >> 24) & 0xFF) {};

	// RGB values
	uint8 r, g, b, a;

	// Default colours
	static constexpr Colour Red() {
		return Colour(0xFFFF0000);
	}

	static constexpr Colour Green() {
		return Colour(0xFF00FF00);
	}

	static constexpr Colour Blue() {
		return Colour(0xFF0000FF);
	}
};