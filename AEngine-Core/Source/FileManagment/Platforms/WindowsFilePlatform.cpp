#include <Windows.h>

#include "FileManagment/Platforms/WindowsFilePlatform.h"
using namespace AE::Core::System;

std::shared_ptr<IFileHandle> WindowsFilePlatform::OpenFile(const std::string& fileName, FileOperation op) {
	return std::make_shared<WindowsFileHandle>(fileName, op);
}

bool WindowsFilePlatform::FileExists(const std::string& fileName) const {
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;

    if (GetFileAttributesExA(fileName.c_str(), GetFileExInfoStandard, &fileInfo)) {
        //false if directory, otherwise file
        return !(fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
    }

    return false;
}