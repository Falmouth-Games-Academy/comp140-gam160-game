#pragma once
#include "Types.h"
#include "String.h"
#include "Math.h"

class DataNode {
public:
	enum Type : uint8 {
		Unknown = 0,
		Byte = 'B',
		Int = 'I',
		Float = 'F',
		Node = 'N',
		Vector3 = 'V',
	};

	union DataNodeValue {
		uint8* byteValues;
		int32* intValues;
		float32* floatValues;
		Vec3* vec3Values;
		DataNode** nodeValues;
	};

public:
	// Constructor
	inline DataNode(Type dataType, const char* tag, int32 numInitialValues = 0);

	// Destructor: frees subnodes if applicable
	inline ~DataNode();

public:
	// Adds a node to the data, returning the new node. Works only if the dataType == Node.
	DataNode* AddNode(Type dataType, const char* tag, int32 numInitialValues = 0);

public:
	// Returns the modifiable values
	inline const DataNodeValue& GetValues();

	// Returns a the first node found with a certain tag (non-recursive)
	inline DataNode* GetNodeByName(const char* name);

	// Reallocates if necessary and sets the number of values
	inline void SetNumValues(int32 numValues);

public:
	// Returns the tag associated with this data node
	inline const char* GetTag() const;

	// Returns the type of data contained by this node
	inline Type GetType() const;

	// Returns the number of values in the data
	inline int GetNum() const;

public:
	typedef struct _iobuf FILE;

	// Attempts to write the node data to a file stream
	bool WriteToFile(FILE* destinationFile) const;

	// Attempts to read the node data from a file stream
	bool ReadFromFile(class DataStream* sourceFile);

public:
	// Maximum length of a data node tag
	static const int maxTagLength = 32;

	// Interval for item allocations
	static const int itemAllocInterval = 16;

private:
	// Tag associated with this data node
	StaticString<maxTagLength> tag;

	// Type of value
	Type type;

	// Values contained in this data node
	DataNodeValue values;

	// Number of values contained in this data node
	int32 numValues;
};

// Text-based data file for maps and object placements, etc
class DataFile {
public:
	// Loads a data file and returns whether successful
	bool Load(const char* filename);

	// Saves to a data file
	bool Save(const char* filename);

	// Returns the root node of this file
	inline DataNode& GetRootNode();

private:
	bool8 isOpen = false;

	DataNode rootNode{DataNode::Node, "Root", 0};
};

// Input data stream for data file interpretation
class DataStream {
public:
	// Creates a data stream from a file
	DataStream(const char* filename);

	// Frees data stream
	~DataStream();

public:
	// Returns the current string from the data pointer
	char* Get() {
		return &stream[index];
	}

	// Advances the data pointer
	char* Advance(int offset) {
		// Update the pointer, clamped between 0 and length
		index += offset;

		if (index < 0) {
			index = 0;
		} else if (index >= length) {
			index = length;
		}

		// Return the stream from this new position
		return &stream[index];
	}

	// Advances until *any* of the chars within findChars are discovered, then skips past them
	inline char* AdvancePast(const char* findChars) {
		int numFindChars = strlen(findChars);

		// Advance until the characters are reached
		for (index; index < length; ++index) {
			for (int c = 0; c < numFindChars; ++c) {
				if (stream[index] == findChars[c]) {
					goto BreakLoop;
				}
			}
		}

		BreakLoop:
		// Advance past the characters
		for (index; index < length; ++index) {
			for (int c = 0; c < numFindChars; ++c) {
				if (stream[index] == findChars[c]) {
					goto ContinueLoop;
				}
			}

			break;
			ContinueLoop:;
		}
	}

	// Advances until *any* of the chars within findChars are discovered, then skips past the first one
	inline char* AdvancePastSingle(const char* findChars) {
		int numFindChars = strlen(findChars);

		// Advance until the characters are reached
		for (index; index < length; ++index) {
			for (int c = 0; c < numFindChars; ++c) {
				if (stream[index] == findChars[c]) {
					goto BreakLoop;
				}
			}
		}

		// Not found
		return stream;

		// Found, skip once
		BreakLoop:
		++index;

		return stream;
	}

public:
	int Length() const {
		return length;
	}

private:
	// Text data contained by this stream
	char* stream;

	// Length of the text data, not including the null terminator
	int32 length;

	// Current index of the data pointer
	int32 index;
};

DataNode& DataFile::GetRootNode() {
	return rootNode;
}

DataNode::DataNode(DataNode::Type type_, const char* tag_, int32 numInitialValues) : type(type_), tag(tag_), values({nullptr}), numValues(0) {
	if (numInitialValues > 0) {
		// Initialise with a set of blank values
		SetNumValues(numInitialValues);
	}
}

DataNode::~DataNode() {
	// Delete subnodes
	if (type == Node) {
		for (int i = 0; i < numValues; ++i) {
			delete values.nodeValues[i];
		}
	}

	// Delete value list (type-independent)
	delete[] values.byteValues;
}

inline const DataNode::DataNodeValue& DataNode::GetValues() {
	return values;
}

inline DataNode* DataNode::GetNodeByName(const char* name) {
	if (type != Node) {
		// Well this ain't gonna work
		return nullptr;
	}

	// Find the node with the matching name if possible
	for (int i = 0; i < numValues; ++i) {
		if (!strcmp(values.nodeValues[i]->GetTag(), name)) {
			return values.nodeValues[i];
		}
	}

	// Nothing found
	return nullptr;
}

void DataNode::SetNumValues(int32 newNumValues) {
	// Reallocate the list
	if (newNumValues > numValues) {
		int bytesPerItem = 0;

		// Determine the size needed per item
		switch (type) {
			case Byte: bytesPerItem = 1; break;
			case Float: bytesPerItem = sizeof (float32); break;
			case Node: bytesPerItem = sizeof (DataNode*); break;
			case Int: bytesPerItem = sizeof (int32); break;
			case Vector3: bytesPerItem = sizeof (Vec3); break;
		}

		// Alloc and copy to the new data
		uint8* newData = new uint8[newNumValues * bytesPerItem];

		memcpy(newData, values.byteValues, numValues * bytesPerItem);

		// Delete the old data
		delete values.byteValues;

		// Reassign
		values.byteValues = newData;
	}

	// Uninitialise node types
	if (type == Node) {
		if (newNumValues < numValues) {
			// Delete the unused nodes
			for (int i = newNumValues; i < numValues; ++i) {
				delete values.nodeValues[i];
			}
		}
	}

	// Update numValues
	numValues = newNumValues;
}

const char* DataNode::GetTag() const {
	return tag;
}

DataNode::Type DataNode::GetType() const {
	return type;
}

int DataNode::GetNum() const {
	return numValues;
}