#pragma once
#include "String.h"

class FileDialog {
public:
	enum FileDialogType {
		OpenFile = 0,
		SaveFile = 1
	};

public:
	// Construct the dialog with the essential settings
	// Filter should be provided in the following format: "Image files/*PNG;*BMP;/All files (*.*)/*.*"
	FileDialog(FileDialogType type_, const char* filter_, const char* defaultExtension_ = "dat") 
		: type(type_), filter(filter_), filename(), defaultExtension(defaultExtension_) {};

	// Opens the dialog
	bool Open();

	// Returns the filename retrieved by the user
	const char* GetResult() const;

private:
	FileDialogType type;

	StaticString<50> defaultExtension;
	StaticString<300> filter;
	StaticString<300> filename;
};