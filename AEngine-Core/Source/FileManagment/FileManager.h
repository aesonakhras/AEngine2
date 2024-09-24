#pragma once
#include <memory>
#include <string>
#include "Utils/Singleton.h"
#include "FileManagment/IFileHandle.h"
#include "Platforms/IFilePlatform.h"


#if defined(_WIN64)
#include "Platforms/WindowsFilePlatform.h"
#endif

namespace AE::System {
	class FileManager : public AE::Utils::Singleton<FileManager> {
		public:
			std::shared_ptr<IFileHandle> GetFile(std::string fileName, FileOperation op);
			bool FileExists(std::string fileName);

		protected:


		private:
			void CreateFilePlatform();
			std::shared_ptr<IFilePlatform> platformFile;

			void initialize() {};
			void shutdown() {};

			FileManager();
			~FileManager() = default;

			friend class AE::Utils::Singleton<FileManager>;

	};
}