#pragma once
#include "../FileManagment/IFileHandle.h"
#include <Windows.h>
#include "../Debug.h"

namespace AE::Core::System {
	class WindowsFileHandle : public IFileHandle {
	public:
		WindowsFileHandle(const std::string fileName, FileOperation op);
		std::vector<char> Read(size_t size) override final;
		std::vector<char> ReadAll() override final;
		void Write(const std::vector<char>& data) override final;
		bool IsOpen() const override final;
		~WindowsFileHandle();

	private:
		HANDLE fileHandle;
		bool writeMode;
	};
}