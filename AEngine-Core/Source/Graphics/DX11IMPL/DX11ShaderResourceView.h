#pragma once
#include <memory>
#include <d3d11.h>
#include <wrl/client.h>

#include "Graphics/IShaderResourceView.h"

namespace AE::Graphics {
	//forward declares
	class DX11TextureResource;

	class DX11ShaderResourceView : public IShaderResourceView {
	public:
		DX11ShaderResourceView() = delete;
		DX11ShaderResourceView(
			Microsoft::WRL::ComPtr<ID3D11DeviceContext>deviceContext,
			Microsoft::WRL::ComPtr <ID3D11Device> device,
			std::shared_ptr<DX11TextureResource> textureResource
		);
		virtual ~DX11ShaderResourceView() {};

		virtual void Bind(unsigned int slot) override;
		virtual void Unbind(unsigned int slot) override {};
	private:

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	};
}