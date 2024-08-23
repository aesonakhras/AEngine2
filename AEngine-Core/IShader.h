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
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
	virtual ~IShader() = default;
};