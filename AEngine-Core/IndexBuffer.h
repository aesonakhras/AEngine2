#pragma once
#include <d3d11.h>
#include <iostream>

class IndexBuffer {
public:
	IndexBuffer(ID3D11Device* device, const void* data, unsigned int dataSize);
	void Bind(ID3D11DeviceContext* deviceContext, DXGI_FORMAT format, UINT offset);
	~IndexBuffer();

private:
	ID3D11Buffer* pIBuffer = nullptr;
};