#pragma once
#include <memory>

#include "FileManagment/Platforms/IFilePlatform.h"
#include "FileManagment/WindowsFileHandle.h"

namespace AE::Core::System {
	class WindowsFilePlatform : public IFilePlatform
	{
	public:
		WindowsFilePlatform() {};
		~WindowsFilePlatform() {};

		std::shared_ptr<IFileHandle> OpenFile(const std::string& fileName, FileOperation op) override;
		bool FileExists(const std::string& fileName) const override;

	};
}