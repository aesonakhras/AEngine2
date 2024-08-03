#pragma once

namespace AEngine {
	namespace Graphics {
		enum class ShaderType {
			Vertex,
			Fragment
		};
	}
}

class IShader {
public:
	virtual void Bind() = 0;
	virtual void UnBind() = 0;
	virtual ~IShader() = default;
};