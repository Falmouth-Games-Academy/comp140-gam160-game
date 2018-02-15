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

// Reliably-sized boolean type
typedef __int8 bool8;

// Unreliably-sized float because floats are unreliable
typedef float float32;