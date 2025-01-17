#pragma once
#include <d3d11.h>
#include <dxgidebug.h>
#include <DirectXMath.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <wrl/client.h>


#include "Graphics/TextureCreateInfo.h"

#include "Graphics/IGLI.h"

#include "Graphics/IVertexDescription.h"
#include "Graphics/VertexAttribute.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace AE::Graphics {
	//forward declare of classes
	class DX11ShaderObject;
	class DX11Buffer;

	class DX11GLI : public IGLI {

	public:
		///////////////////////////From the IGLI
		virtual void Init(const DeviceCreateInfo& info) final override;
		virtual void ShutDown() final override;

		virtual void StartFrame() final override;
		virtual void PresentFrame() final override;

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

		virtual void RecompileVertexShader(const void* data, size_t dataSize, std::shared_ptr<IVertexShader>& vertexShader) final override;
		virtual void RecompileFragmentShader(const void* data, size_t dataSize, std::shared_ptr <IFragmentShader>& fragmentShader) final override;

		virtual void ChangeDepthState(bool isSkybox) final override;

		//Binding
		virtual void BindBuffer(const std::shared_ptr<IBuffer>& ib) final override;
		///////////////////////////End of the IGLI functions

	private:
		//creation helper functions
		void createDevice();
		void createSwapChainAndBackBuffer(const DeviceCreateInfo& info);
		void setupViewport(const DeviceCreateInfo& info);
		void createRasterStates();
		void setRasterState(std::string state);


		std::shared_ptr<DX11ShaderObject> CreateShaderObject(const void* data, size_t dataSize, std::string entryPoint, std::string shaderTarget);
		void setupDepthStencilState();
		void D3DCreateCall(HRESULT hresult, std::string failInfo);
		void PrintHResult(HRESULT result);

		//TODO: Find another solution for this
		typedef float RGBA[4];

		Microsoft::WRL::ComPtr <ID3D11Device> m_device = nullptr;
		Microsoft::WRL::ComPtr <ID3D11DeviceContext> m_deviceContext = nullptr;
		Microsoft::WRL::ComPtr <IDXGISwapChain> m_swapChain = nullptr;
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> m_backBuffer = nullptr;

		Microsoft::WRL::ComPtr <ID3D11DepthStencilState> m_defaultDepthStencilState = nullptr;
		Microsoft::WRL::ComPtr <ID3D11DepthStencilState> m_skyBoxStencilState = nullptr;

		ID3D11RenderTargetView* m_renderTargetView = nullptr;
		ID3D11ShaderResourceView* m_shaderResourceView = nullptr;
		ID3D11DepthStencilView* m_depthStencilView = nullptr;
		ID3D11SamplerState* m_samplerState = nullptr;


		std::shared_ptr<DX11Buffer> m_quadVertexBuffer = nullptr;
		std::shared_ptr<DX11Buffer> m_quadIndexBuffer = nullptr;
		std::shared_ptr<IVertexShader> m_screenQuadVertexShader;
		std::shared_ptr<IFragmentShader> m_screenQuadFragmentShader;

		std::unordered_map < std::string, Microsoft::WRL::ComPtr<ID3D11RasterizerState> > rasterStates;

		void setUpScreenQuad();


		//hard coded screen stuff
		const std::string ScreenQuadVertexShader = R"(
			struct VertexInputType {
				float3 position : POSITION;
				float2 texCoord : TEXCOORD0;
			};

			struct PixelInputType {
				float4 position : SV_POSITION;
				float2 texCoord : TEXCOORD0;
			};

			PixelInputType VShader(VertexInputType input) {
				PixelInputType output;
				output.position = float4(input.position, 1.0f);
				output.texCoord = input.texCoord;
				return output;
			}
		)";

		const std::string ScreenQuadFragmentShader = R"(
			Texture2D shaderTexture : register(t0);
			SamplerState sampleType : register(s0);

			struct PixelInputType {
				float4 position : SV_POSITION;
				float2 texCoord : TEXCOORD0;
			};

			float4 PShader(PixelInputType input) : SV_TARGET {
				//return float4(1,0,0,0);
				return shaderTexture.Sample(sampleType, input.texCoord);
			}
		)";

		struct ScreenQuadVertex {
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 texCoord;
		};

		struct ScreenQuadVertexDescription : IVertexDescription
		{
			const static std::vector<VertexAttribute>& Get() {

				static std::vector<VertexAttribute> attributes{
					{"POSITION", 0, TextureFormat::RGB32F, 0},
					{"TEXCOORD", 0, TextureFormat::RG32F, 12}
				};

				return attributes;
			}
		};


		ScreenQuadVertex m_screenQuadVertices[4] = {
			{ DirectX::XMFLOAT3(-1.0f,  1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
			{ DirectX::XMFLOAT3(1.0f,  1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
			{ DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }
		};

		int m_screenQuadIndices[6] = {
			0,1,2,
			0,2,3
		};
	};
}