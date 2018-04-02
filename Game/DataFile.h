#pragma once
#include "Types.h"
#include "String.h"
#include "Math.h"

class DataNode {
public:
	enum Type : uint8 {
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

	// Returns the modifiable values
	inline const DataNodeValue& GetValues();

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
	// Attempts to write the node data to a file
	typedef struct _iobuf FILE;
	bool WriteToFile(FILE* destinationFile) const;

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