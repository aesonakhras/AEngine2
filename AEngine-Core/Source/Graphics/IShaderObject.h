#pragma once
#include <string>

namespace AEngine {
	namespace Graphics {
		enum class ShaderType {
			Vertex,
			Fragment
		};
	}
}

class IShaderObject {
public:	
	virtual ~IShaderObject() = default;

	template<typename T>
	inline T* Get() {
		return static_cast<T*>(GetRawObject());
	}

	protected:
		virtual void* GetRawObject() = 0;
};