#pragma once
#include <memory>
#include <string>
#include "FileManagment/IFileHandle.h"

namespace AE::Core::System {
	class IFilePlatform {
		public:
			virtual ~IFilePlatform() = default;

			//provides file handles
			virtual std::shared_ptr<IFileHandle> OpenFile(const std::string& fileName, FileOperation op) = 0;
			virtual bool FileExists(const std::string& fileName) const  = 0;
	};
}