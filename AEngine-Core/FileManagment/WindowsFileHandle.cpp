#include "../FileManagment/WindowsFileHandle.h"

using namespace AE::Core::System;

WindowsFileHandle::WindowsFileHandle(const std::string fileName, FileOperation op) :
fileHandle(INVALID_HANDLE_VALUE), writeMode(op == FileOperation::Write){
	DWORD accessType = writeMode ?  GENERIC_WRITE : GENERIC_READ;
	DWORD disposition = writeMode ? CREATE_ALWAYS : OPEN_EXISTING;

	fileHandle = CreateFileA(fileName.c_str(),
									accessType,
									0,
									NULL,
									disposition,
									FILE_ATTRIBUTE_NORMAL,
									NULL
									);

	if (fileHandle == INVALID_HANDLE_VALUE) {
		AE::Core::Debug::LogError("Unable to Open File: " + fileName);
	}

}

std::vector<char> WindowsFileHandle::ReadAll() {
	if (fileHandle == INVALID_HANDLE_VALUE || writeMode) {
		return {};
	}

	LARGE_INTEGER filesize;

	if (!GetFileSizeEx(fileHandle, &filesize)) {
		AE::Core::Debug::LogError("Unable to get file size.");
		return{};
	}
	else {
		return Read(static_cast<size_t>(filesize.QuadPart));
	}
}

std::vector<char> WindowsFileHandle::Read(size_t size) {
	std::vector<char> buffer;
	
	if (fileHandle == INVALID_HANDLE_VALUE || writeMode) {
		AE::Core::Debug::LogError("File not open, or incorrect File Operation");
		buffer;
	}

	buffer.resize(size);
	DWORD bytesRead;

	bool result = ReadFile(fileHandle, buffer.data(), static_cast<DWORD>(size),
							&bytesRead, NULL);

	if (result) {
		buffer.resize(bytesRead);
	}
	else {
		buffer.clear();
		AE::Core::Debug::LogError("Error in reading file");
	}

	return buffer;
}

void WindowsFileHandle::Write(const std::vector<char>& data) {
	if (fileHandle == INVALID_HANDLE_VALUE || !writeMode) {
		AE::Core::Debug::LogError("File not open, or incorrect File Operation");
	}

	DWORD bytesWritten;
	bool result = WriteFile(fileHandle, data.data(), static_cast<DWORD>(data.size()), &bytesWritten, NULL);

	if (!result) {
		AE::Core::Debug::LogError("Failed to write to file");
	}
}

bool WindowsFileHandle::IsOpen() const {
	return fileHandle != INVALID_HANDLE_VALUE;
}

WindowsFileHandle::~WindowsFileHandle() {
	if (fileHandle != INVALID_HANDLE_VALUE) {
		CloseHandle(fileHandle);
		fileHandle = INVALID_HANDLE_VALUE;
	}
}