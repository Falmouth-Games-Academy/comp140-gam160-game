#include "FileDialog.h"

#include <Windows.h>

bool FileDialog::Open() {
	OPENFILENAME ofn;

	// Convert our filter to one that Windows is happy with. We need to do this becasue StaticStrings aren't programmed to copy beyond the null terminator!
	auto convertedFilter = filter;

	for (int i = 0; ; ++i) {
		if (convertedFilter[i] == '/') {
			convertedFilter[i] = '\0';
		} else if (convertedFilter[i] == '\0') {
			convertedFilter[i + 1] = '\0';
			break;
		}
	}

	// Setup dialog settings
	char tempFilename[MAX_PATH] = "";

	memset(&ofn, 0, sizeof (ofn));
	ofn.lStructSize = sizeof (ofn);
	ofn.lpstrFile = tempFilename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = convertedFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrTitle = "Select file";
	ofn.lpstrDefExt = defaultExtension;
	ofn.Flags = OFN_NOCHANGEDIR; // Who thought this was a good idea??	

	// Display the dialog to the user and wait for a response
	bool retVal;
	
	if (type == FileDialogType::OpenFile) {
		ofn.Flags |= OFN_FILEMUSTEXIST;

		retVal = GetOpenFileName(&ofn);
	} else {
		ofn.Flags |= OFN_OVERWRITEPROMPT;

		retVal = GetSaveFileName(&ofn);
	}

	int err = CommDlgExtendedError();

	// If successful, copy the returned file path to 'filename'
	if (retVal) {
		filename = tempFilename;
	}

	return retVal;
}

const char* FileDialog::GetResult() const {
	return filename;
}