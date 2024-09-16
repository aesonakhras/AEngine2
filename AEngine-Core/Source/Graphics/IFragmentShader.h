#pragma once
#include <memory>

//forward declares
class IShader;

namespace AE::Graphics {
	class IFragmentShader {
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

	protected:
		std::shared_ptr<IShader> shaderBase;
	};
}