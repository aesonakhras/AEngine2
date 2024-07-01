#pragma once

//TODO: This should be removed if possible:
#include <iostream>
#include <d3d11.h>

//TODO: Turn this into generic buffer class?
class IIndexBuffer {
public:
	size_t IndicesCount;
	size_t IndicesSize;

	IIndexBuffer(size_t IndicesCount, size_t IndicesSize) :
		IndicesCount(IndicesCount),
		IndicesSize(IndicesSize) { };

	virtual ~IIndexBuffer() = default;
};

//the other index buffer will die soon hahahahah
class DX11_IndexBuffer : public IIndexBuffer {
public:
	ID3D11Buffer* Resource = nullptr;

	DX11_IndexBuffer(ID3D11DeviceContext* deviceContext, ID3D11Device* device, const void* data, size_t dataSize);
	unsigned int GetSize();

	virtual ~DX11_IndexBuffer();
};