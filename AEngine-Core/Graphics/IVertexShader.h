#pragma once
#include <memory>



namespace AE::Graphics {
	//forward declares
	class IShader;

	class IVertexShader {
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

	protected:
		std::shared_ptr<IShader> shaderBase;
	};
}