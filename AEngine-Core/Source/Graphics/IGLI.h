#pragma once


//TODO: These should not be here at all
#include <string>
#include <vector>

#include "IBuffer.h"
#include "IShaderResourceView.h"
#include "ISampler.h"
#include "IShaderResourceView.h"
#include "ITextureResource.h"
#include "TextureCreateInfo.h"
#include "IFragmentShader.h"
#include "IVertexShader.h" 

//TODO: Must refactor, this is a D3D11 class.  need to determine abstraction
namespace AE::Graphics {
	//forward declares
	struct VertexAttribute;
	struct DeviceCreateInfo;

	class IGLI {
	public:
		virtual void Init(const DeviceCreateInfo& info) = 0;
		virtual void ShutDown() = 0;

		virtual void StartFrame() = 0;
		virtual void PresentFrame() = 0;

		virtual void Draw(unsigned int size) = 0;

		virtual std::shared_ptr <IBuffer> CreateBuffer(const void* data, size_t count, size_t stride, BufferType bufferType) = 0;

		virtual std::shared_ptr<IVertexShader> CreateVertexShader(
			const void* data,
			size_t dataSize,
			const std::vector<VertexAttribute>& attributes) = 0;

		virtual std::shared_ptr<IFragmentShader> CreateFragmentShader(const void* data,size_t dataSize) = 0;

		virtual std::shared_ptr<AE::Graphics::IShaderResourceView> CreateShaderResourceView(const std::shared_ptr<AE::Graphics::ITextureResource> textureResource) = 0;
		virtual std::shared_ptr<AE::Graphics::ISampler> CreateSampler() = 0;
		virtual std::shared_ptr<AE::Graphics::ITextureResource> CreateTextureResource(const AE::Graphics::TextureCreateInfo& createInfo) = 0;

		virtual void RecompileVertexShader(const void* data, size_t dataSize, std::shared_ptr<IVertexShader>& vertexShader) = 0;
		virtual void RecompileFragmentShader(const void* data, size_t dataSize, std::shared_ptr <IFragmentShader>& fragmentShader) = 0;
		
		//Binding
		virtual void BindBuffer(const std::shared_ptr<IBuffer>& ib) = 0;

		virtual void ChangeDepthState(bool isSkybox) = 0;

		virtual ~IGLI() {};
	};
}