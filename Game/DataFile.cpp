#include "DataFile.h"

#include <cstdio>

bool DataFile::Save(const char* filename) {
	FILE* destFile = fopen(filename, "wb");
	
	// Write the root node to the file (the root node will write all its subnodes recursively)
	if (destFile) {
		rootNode.WriteToFile(destFile);

		fclose(destFile);

		return true;
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
				StaticString<96> vector = StaticString<96>::FromFormat(i == numValues - 1 ? "%.3f,%.3f,%.3f]" : "%.3f,%.3f,%.3f,", values.floatValues[i]);

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