#pragma once

#include <d3d11.h>
#include <wrl/client.h>

class Sampler {
public:
	Sampler(Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr <ID3D11Device> device);
	void Bind();
	~Sampler();
private:
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> m_deviceContext;
	ID3D11SamplerState* m_pSampler = nullptr;
};