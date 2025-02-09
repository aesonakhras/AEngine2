#pragma once
#include <variant>
#include <memory>
#include <d3d11.h>
#include <wrl/client.h>

#include "Graphics/IShaderResourceView.h"
#include "Graphics/TextureCreateInfo.h"

namespace AE::Graphics {
	//forward declares
	class DX11TextureResource;
	struct TextureCreateInfo;

	class DX11ShaderResourceView : public IShaderResourceView {
	public:
		DX11ShaderResourceView() = delete;
		DX11ShaderResourceView(
			Microsoft::WRL::ComPtr<ID3D11DeviceContext>deviceContext,
			Microsoft::WRL::ComPtr <ID3D11Device> device,
			std::shared_ptr<DX11TextureResource> textureResource,
			const AE::Graphics::TextureCreateInfo& info
		);
		virtual ~DX11ShaderResourceView() {};

		virtual void Bind(unsigned int slot) override;
		virtual void Unbind(unsigned int slot) override;

		virtual void BindAsRenderTarget() override;
		virtual void UnBindAsRenderTarget() override;

		virtual void ClearRenderTarget() override;
	private:

		std::variant<
			std::monostate, 
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView>,
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView>
		> m_renderView;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;

		void CreateShaderResourceView(
			std::shared_ptr<DX11TextureResource> textureResource,
			const AE::Graphics::TextureCreateInfo& info
		);

		void CreateDepthStencilView(
			std::shared_ptr<DX11TextureResource> textureResource,
			DXGI_FORMAT format
		);

		TextureUse m_use;

		//TODO: Find another solution for this
		//TODO: Tie this in with DX11_GLI HACK
		typedef float RGBA[4];
	};
}