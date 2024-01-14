#pragma once
#include <d3d11.h>
#include <iostream>

class IndexBuffer {
public:
	IndexBuffer(ID3D11DeviceContext* deviceContext, ID3D11Device* device, const void* data, unsigned int dataSize);
	void Bind(DXGI_FORMAT format, UINT offset);
	unsigned int GetSize();
	~IndexBuffer();

private:
	ID3D11Buffer* m_pIBuffer = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	unsigned int m_indeciesCount;
};

//class IndexBuffer2 {
//	void* data;
//	size_t count;
//	size_t stride;
//};

//class IndexBuffer {
//public:
//	IndexBuffer(ID3D11DeviceContext* deviceContext, ID3D11Device* device, const void* data, unsigned int dataSize);
//	void Bind(DXGI_FORMAT format, UINT offset);
//	unsigned int GetSize();
//	~IndexBuffer();
//
//private:
//	ID3D11Buffer* m_pIBuffer = nullptr;
//	ID3D11DeviceContext* m_deviceContext = nullptr;
//	unsigned int m_indeciesCount;
//};