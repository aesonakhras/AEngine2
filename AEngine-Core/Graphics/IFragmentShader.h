#pragma once
#include <memory>
#include <d3d11.h>

class IShader;

class IFragmentShader {
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

	protected:
		std::shared_ptr<IShader> shaderBase;
};