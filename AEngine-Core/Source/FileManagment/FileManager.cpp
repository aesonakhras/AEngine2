#include "FileManager.h"

using namespace AE::System;

FileManager::FileManager() {
	CreateFilePlatform();
}

std::shared_ptr<IFileHandle> FileManager::GetFile(std::string fileName, FileOperation op) {
	return platformFile->OpenFile(fileName, op);
}

bool FileManager::FileExists(std::string fileName) {
	return platformFile->FileExists(fileName);
}

void FileManager::CreateFilePlatform() {
#if defined(_WIN64)
	platformFile = std::make_shared<WindowsFilePlatform>();
#endif
}

