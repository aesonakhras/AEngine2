#pragma once
#include <d3d11.h>
#include <dxgidebug.h>

#include <string>
#include <vector>
#include <memory>

#include "../IGLI.h"

#include <wrl/client.h>
#include "../Graphics/TextureCreateInfo.h"

//TODO: Lmao what is this
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace AE::Graphics {
	//forward declare of classes
	class DX11ShaderObject;

	class DX11GLI : public IGLI {

	public:
		///////////////////////////From the IGLI
		virtual void Init(const DeviceCreateInfo& info) final override;
		virtual void ShutDown() final override;

		virtual void Clear() final override;
		virtual void Swap() final override;

		virtual void Draw(unsigned int size) final override;

		//Creation
		virtual std::shared_ptr <IBuffer> CreateBuffer(const void* data, size_t count, size_t stride, BufferType bufferType) final override;
		
		virtual std::shared_ptr<AE::Graphics::IShaderResourceView> CreateShaderResourceView(const std::shared_ptr<AE::Graphics::ITextureResource> textureResource) final override;
		virtual std::shared_ptr<AE::Graphics::ISampler> CreateSampler() final override;
		virtual std::shared_ptr<AE::Graphics::ITextureResource> CreateTextureResource(const AE::Graphics::TextureCreateInfo& createInfo) final override;

		virtual std::shared_ptr<IVertexShader> CreateVertexShader(const void* data,
			size_t dataSize,
			const std::vector<VertexAttribute>& attributes) final override;

		virtual std::shared_ptr<IFragmentShader> CreateFragmentShader(const void* data, size_t dataSize) final override;

		//Binding
		virtual void BindBuffer(const std::shared_ptr<IBuffer>& ib) final override;
		///////////////////////////End of the IGLI functions

	private:
		std::shared_ptr<DX11ShaderObject> CreateShaderObject(const void* data, size_t dataSize, std::string entryPoint, std::string shaderTarget);
		void setupDepthStencilState();
		void D3DCreateCall(HRESULT hresult, std::string failInfo);
		void PrintHResult(HRESULT result);

		ID3D11Texture2D* CreateTextureD3D(void* data, unsigned int height, unsigned int width, unsigned int miplevel,
			DXGI_FORMAT format, unsigned int sampleCount, unsigned int BindFlags);

		//TODO: Find another solution for this
		typedef float RGBA[4];

		Microsoft::WRL::ComPtr <ID3D11Device> m_device = nullptr;
		Microsoft::WRL::ComPtr <ID3D11DeviceContext> m_deviceContext = nullptr;
		Microsoft::WRL::ComPtr <ID3D11Texture2D> m_depthTexture = nullptr;
		Microsoft::WRL::ComPtr <IDXGISwapChain> m_swapChain = nullptr;
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> m_backBuffer = nullptr;
		Microsoft::WRL::ComPtr <ID3D11DepthStencilView> m_pDSV = nullptr;
	};
}