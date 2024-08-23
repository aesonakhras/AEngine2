#pragma once
#include "DeviceCreateInfo.h"
#include "IBuffer.h"

//TODO: These should not be here at all
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include "IShader.h"

//TODO: Must refactor, this is a D3D11 class.  need to determine abstraction
namespace AECore {
	class IGLI {
	public:
		//Temp Solution, ensure that any API specific code is out of this soon

		virtual Microsoft::WRL::ComPtr <ID3D11Device>  GetDevice() = 0;
		virtual Microsoft::WRL::ComPtr <ID3D11DeviceContext> GetDeviceContext() = 0;

		virtual void Init(AECore::DeviceCreateInfo info) = 0;
		virtual void ShutDown() = 0;

		virtual void Clear() = 0;
		virtual void Swap() = 0;

		virtual std::shared_ptr <IBuffer> CreateBuffer(const void* data, size_t count, size_t stride, AEngine::Graphics::BufferType bufferType) = 0;
		virtual std::shared_ptr<IShader> CreateShader(std::string shaderName, AEngine::Graphics::ShaderType shaderType) = 0;
		
		//Binding
		virtual void BindBuffer(const std::shared_ptr<IBuffer>& ib) = 0;

		virtual ~IGLI() {};
	};
}