#pragma once
#include <d3d11.h>
#include <iostream>


//the other index buffer will die soon hahahahah
struct IndexBuffer {
	void* ResourcePTR;

	//TODO: Change to a better DS later
	size_t IndicesCount;
	size_t IndicesSize;

	IndexBuffer(void* resourcePTR, size_t indicesCount, size_t indicesSize) :
		ResourcePTR(resourcePTR),
		IndicesCount(indicesCount),
		IndicesSize(indicesSize)
	{ }
};



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