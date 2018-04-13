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

	// Functions
	// Returns a lerped copy of a colour
	inline Colour Lerped(const Colour& otherColour, float factor) const;

	// Common colours
	static constexpr Colour Red() {
		return Colour(0xFFFF0000);
	}

	static constexpr Colour Green() {
		return Colour(0xFF00FF00);
	}

	static constexpr Colour Blue() {
		return Colour(0xFF0000FF);
	}

	static constexpr Colour Black() {
		return Colour(0xFF000000);
	}

	static constexpr Colour White() {
		return Colour(0xFFFFFFFF);
	}
};

Colour Colour::Lerped(const Colour& otherColour, float factor) const {
	int src = (int)((1.0f - factor) * 255.0f), dest = (int)(factor * 255.0f);

	// r * src / 255 + g * dest / 255 == (r * src + g * dest) / 255
	return Colour((r * src + otherColour.r * dest) / 255, (g * src + otherColour.g * dest) / 255, 
				  (b * src + otherColour.b * dest) / 255, (a * src + otherColour.a * dest) / 255);
}