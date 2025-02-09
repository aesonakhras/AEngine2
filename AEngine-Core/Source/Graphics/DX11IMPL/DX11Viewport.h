#pragma once
#include <d3d11.h>
#include <wrl/client.h>

#include "Graphics/IViewport.h"


namespace AE::Graphics {
	class DX11Viewport : public IViewport {
	public:
		DX11Viewport (
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
			const AE::Graphics::ViewPortCreateInfo& info
		);
		
		virtual void Bind() final override;

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
		D3D11_VIEWPORT viewport;
	};
}