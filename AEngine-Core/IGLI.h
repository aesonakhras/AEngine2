#pragma once
#include "DeviceCreateInfo.h"
#include "Graphics/IBuffer.h"

//TODO: These should not be here at all
#include <d3d11.h>	//<-this will fuck die soon
#include <wrl/client.h> //<-this will fuck die soon


#include <string>
#include "Graphics/IShaderObject.h"
#include "Graphics/IShaderResourceView.h"
#include "Graphics/ISampler.h"
#include "Graphics/IShaderResourceView.h"
#include "Graphics/ITextureResource.h"

#include "Graphics/TextureCreateInfo.h"

#include "Graphics/IFragmentShader.h"
#include "Graphics/IVertexShader.h" 



//TODO: Must refactor, this is a D3D11 class.  need to determine abstraction
namespace AE::Graphics {
	//forward declares
	struct VertexAttribute;

	class IGLI {
	public:
		//Temp Solution, ensure that any API specific code is out of this soon

		virtual Microsoft::WRL::ComPtr <ID3D11Device>  GetDevice() = 0;
		virtual Microsoft::WRL::ComPtr <ID3D11DeviceContext> GetDeviceContext() = 0;

		virtual void Init(AECore::DeviceCreateInfo info) = 0;
		virtual void ShutDown() = 0;

		virtual void Clear() = 0;
		virtual void Swap() = 0;

		virtual std::shared_ptr <IBuffer> CreateBuffer(const void* data, size_t count, size_t stride, BufferType bufferType) = 0;

		virtual std::shared_ptr<IVertexShader> CreateVertexShader(
			const void* data,
			size_t dataSize,
			const std::vector<VertexAttribute>& attributes) = 0;

		virtual std::shared_ptr<IFragmentShader> CreateFragmentShader(const void* data,size_t dataSize) = 0;

		virtual std::shared_ptr<AE::Graphics::IShaderResourceView> CreateShaderResourceView(const std::shared_ptr<AE::Graphics::ITextureResource> textureResource) = 0;
		virtual std::shared_ptr<AE::Graphics::ISampler> CreateSampler() = 0;
		virtual std::shared_ptr<AE::Graphics::ITextureResource> CreateTextureResource(const AE::Graphics::TextureCreateInfo& createInfo) = 0;
		
		//Binding
		virtual void BindBuffer(const std::shared_ptr<IBuffer>& ib) = 0;

		virtual ~IGLI() {};
	};
}