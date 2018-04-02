#pragma once

#include <cstring>
#include <cstdarg>

template <int MaxLength>
class StaticString {
public:
	// Constructor: Makes an empty string
	StaticString();
	StaticString(const char* str);

	// Copy assignment
	StaticString<MaxLength>& operator=(const char* strIn);

	// C-string conversion operators
	inline operator char*();
	inline operator const char*() const;

	// Bool conversion operator, so that if (StaticString) can be used to check empty strings
	inline operator bool() const;
	
public:
	// Returns the length of the string up to the null terminator, in characters
	inline int Length() const;

public:
	// Constructs string based on printf-style format
	static StaticString<MaxLength> FromFormat(const char* format, ...);
private:
	char data[MaxLength];
};

template<int MaxLength> StaticString<MaxLength>::StaticString() {
	data[0] = '\0';
}

template<int MaxLength> inline StaticString<MaxLength>::StaticString(const char* str) {
	*this = str;
}

template<int MaxLength> StaticString<MaxLength> StaticString<MaxLength>::FromFormat(const char* format, ...) {
	StaticString<MaxLength> newString;

	// Grab the variadic arguments
	va_list args;

	va_start(args, format);

	// Use vsprintf_s because we can (warning: Microsoft-specific?)
	vsprintf_s(newString, MaxLength, format, args);

	// Ungrab the variadic functions. This isn't totally important so much as we don't ever know if it's important, which is important to know.
	// I should've taken a philosophy course
	va_end(args);

	// Return the newly instantiated string (I wonder how efficient this is...?)
	return newString;
}

template<int MaxLength> StaticString<MaxLength>& StaticString<MaxLength>::operator=(const char* strIn) {
	// Copy string and add implicit null terminator (because strncpy is naughty and doesn't do it for us)
	strncpy_s(data, strIn, MaxLength - 1);
	data[MaxLength - 1] = '\0';
	return *this;
}

template<int MaxLength> StaticString<MaxLength>::operator char*() {
	return data;
}

template<int MaxLength> StaticString<MaxLength>::operator const char*() const {
	return data;
}

template<int MaxLength> StaticString<MaxLength>::operator bool() const {
	return data[0] != '\0';
}

template<int MaxLength> int StaticString<MaxLength>::Length() const {
	return strlen(data); // gg ez lawl
}