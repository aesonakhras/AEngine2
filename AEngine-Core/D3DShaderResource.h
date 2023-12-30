#pragma once
#include <d3d11.h>

class D3DShaderResource {
public:
	D3DShaderResource(ID3D11DeviceContext* deviceContext, ID3D11Device* device, ID3D11Texture2D* texture, D3D11_SRV_DIMENSION viewDimension);

	void Bind();
private:
	ID3D11ShaderResourceView* m_shaderResourceView;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Device* m_device;
};