#include "DX11Viewport.h"

using namespace AE::Graphics;

DX11Viewport::DX11Viewport(
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	const AE::Graphics::ViewPortCreateInfo& info
) : m_deviceContext(deviceContext) {
	viewport = {};

	viewport.TopLeftX = info.x;
	viewport.TopLeftY = info.y;
	viewport.Width = info.width;
	viewport.Height = info.height;
	viewport.MinDepth = info.minDepth;
	viewport.MaxDepth = info.maxDepth;
}

void DX11Viewport::Bind() {
	m_deviceContext->RSSetViewports(1, &viewport);
}