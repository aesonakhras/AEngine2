#pragma once
#include <vector>
#include <string>

namespace AE::System {
	enum class FileOperation {
		Read,
		Write
	};
	
	class IFileHandle {
	public:
		//open and close will be handled by RAII so ensure that the constructor has a filename and FileOperation, or not.
		virtual std::vector<char> Read(size_t size) = 0;
		virtual std::vector<char> ReadAll() = 0;
		virtual void Write(const std::vector<char>& data) = 0;
		virtual bool IsOpen() const = 0;
		virtual ~IFileHandle() = default;
	};
}