#pragma once

#include <d3d11.h>

class Sampler {
public:
	Sampler(ID3D11DeviceContext* deviceContext, ID3D11Device* device);
	void Bind();
	~Sampler();
private:
	ID3D11DeviceContext* m_deviceContext;
	ID3D11SamplerState* m_pSampler = nullptr;
};