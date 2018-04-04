#include "DataFile.h"

#include <cstdio>

bool DataFile::Save(const char* filename) {
	FILE* destFile = fopen(filename, "wb");
	
	// Write the root node to the file (the root node will write all its subnodes recursively)
	if (destFile) {
		bool success = rootNode.WriteToFile(destFile);

		fclose(destFile);

		return success;
	} else {
		return false;
	}
}

bool DataFile::Load(const char* filename) {
	DataStream sourceFile(filename);

	if (sourceFile.Length()) {
		bool success = rootNode.ReadFromFile(&sourceFile);

		return success;
	} else {
		return false;
	}
}

DataNode* DataNode::AddNode(Type dataType, const char* tag, int32 numInitialValues) {
	// Add and return a new node
	SetNumValues(numValues + 1);

	values.nodeValues[numValues - 1] = new DataNode(dataType, tag, numInitialValues);
	return values.nodeValues[numValues - 1];
}

bool DataNode::WriteToFile(FILE* destFile) const {
	StaticString<maxTagLength + 3> blockOpener = StaticString<maxTagLength + 3>::FromFormat("%s:%c[", (const char*)tag, type);

	// Write the data in text form
	switch (type) {
		case Float: {
			// Write text floats
			fwrite((char*)blockOpener, 1, blockOpener.Length(), destFile);

			for (int i = 0; i < numValues; ++i) {
				StaticString<32> number = StaticString<32>::FromFormat(i == numValues - 1 ? "%.3f]" : "%.3f,", values.floatValues[i]);
				
				fwrite((char*)number, 1, number.Length(), destFile);
			}

			return true;
		}
		case Vector3: {
			// Write vector with text float XYZ components
			fwrite((char*)blockOpener, 1, blockOpener.Length(), destFile);

			for (int i = 0; i < numValues; ++i) {
				StaticString<96> vector = StaticString<96>::FromFormat(i == numValues - 1 ? "%.3f,%.3f,%.3f]" : "%.3f,%.3f,%.3f,", 
					values.vec3Values[i].x, values.vec3Values[i].y, values.vec3Values[i].z);

				fwrite((char*)vector, 1, vector.Length(), destFile);
			}

			return true;
		}
		case Node: {
			// Write node data using DataNode::WriteToFile (deja vu!?)
			fwrite((char*)blockOpener, 1, blockOpener.Length(), destFile);

			for (int i = 0; i < numValues; ++i) {
				values.nodeValues[i]->WriteToFile(destFile);
			}

			fwrite("]", 1, 1, destFile);

			return true;
		}
	}

	return false;
}

bool DataNode::ReadFromFile(DataStream* sourceFile) {
	StaticString<maxTagLength> tag = "";
	Type type = Unknown;
	int curTagIndex = 0;
	char* stream = sourceFile->Get();

	// Read chars from sourceFile until a colon is reached
	while (stream[curTagIndex] != ':' && curTagIndex < maxTagLength) {
		tag[curTagIndex] = stream[curTagIndex++];
	}

	if (curTagIndex >= maxTagLength || !curTagIndex) {
		// Invalid tag
		goto LoadError;
	}

	// Remove the colon from the tag
	tag[curTagIndex] = '\0';

	// Read the type
	type = (Type)stream[curTagIndex + 1];

	// Safety check: ensure there's an opening bracket following the type
	if (stream[curTagIndex + 2] != '[') {
		// No opening bracket found
		goto LoadError;
	}

	// Advance the stream pointer
	stream = sourceFile->Advance(curTagIndex + 3);

	// Read the data, if possible
	switch (type) {
		case Node: {
			this->type = type;

			while (sourceFile->Get()[0] != ']' && sourceFile->Get()[0] != '\0') {
				if (!AddNode(Unknown, "", 0)->ReadFromFile(sourceFile)) {
					goto LoadError; // This is very harmful considering
				}
			}

			// Ensure the closing bracket was safely reached
			char* blargh = sourceFile->Get();
			if (sourceFile->Get()[0] == ']') {
				sourceFile->Advance(1);
			} else {
				goto LoadError;
			}

			break;
		}
		case Vector3: {
			this->type = type;

			// Count the number of values to be read
			int numTerminators = 0;
			for (int index = 0; stream[index] != '\0'; ++index) {
				if (stream[index] == ',') {
					++numTerminators;
				} else if (stream[index] == ']') {
					++numTerminators;
					break;
				}
			}

			// Make sure there's a multiple of 3 values ('cause these are 3D vectors)
			if (numTerminators != (numTerminators / 3) * 3) {
				goto LoadError;
			}

			// Interpret the values
			SetNumValues(numTerminators / 3);
			for (int i = 0; i < numTerminators; ++i) {
				char* buffer = sourceFile->Get();
				int scanResult = 0;
				
				if (i == numTerminators - 1) {
					scanResult = sscanf(buffer, "%f,", &values.floatValues[i]);
				} else {
					scanResult = sscanf(buffer, "%f]", &values.floatValues[i]);
				}

				if (scanResult == 1) {
					sourceFile->AdvancePastSingle(",]");
					continue;
				} else {
					goto LoadError;
				}
			}

			break;
		}
		default: {
			goto LoadError;
		}
	}

	// Success! Update variables
	this->tag = tag;
	return true;

LoadError:
	// Cleanup after failed load

	return false;
}

DataStream::DataStream(const char* filename) : stream(nullptr), length(0), index(0) {
	FILE* sourceFile = fopen(filename, "rb");

	if (sourceFile) {
		// Get the length of the file
		int fileLength;
		fseek(sourceFile, 0, SEEK_END);
		fileLength = ftell(sourceFile);
		fseek(sourceFile, 0, SEEK_SET);

		// Allocate the stream buffer
		stream = new char[fileLength + 1];

		// Read the file into the stream buffer
		fread(stream, 1, fileLength, sourceFile);
		stream[fileLength] = '\0'; // append null terminator

		// Update length variable
		length = fileLength;

		// Done!
		fclose(sourceFile);
	}
}

DataStream::~DataStream() {
	delete[] stream;
}