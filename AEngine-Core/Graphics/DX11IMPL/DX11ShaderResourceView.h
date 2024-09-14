#pragma once
#include "../IShaderResourceView.h"
#include <d3d11.h>
#include <wrl/client.h>

namespace AE::Graphics {
	class DX11ShaderResourceView : public IShaderResourceView {
	public:
		DX11ShaderResourceView() = delete;
		DX11ShaderResourceView(Microsoft::WRL::ComPtr<ID3D11DeviceContext>deviceContext, Microsoft::WRL::ComPtr <ID3D11Device> device, ID3D11Resource* texture, D3D11_SRV_DIMENSION viewDimension);
		virtual ~DX11ShaderResourceView() {};

		virtual void Bind(unsigned int slot) override;
		virtual void Unbind(unsigned int slot) override {};


	//TODO: Make these smart
	private:

		Microsoft::WRL::ComPtr < ID3D11ShaderResourceView> m_shaderResourceView;
		Microsoft::WRL::ComPtr < ID3D11DeviceContext> m_deviceContext;
		Microsoft::WRL::ComPtr < ID3D11Device> m_device;
	};
}