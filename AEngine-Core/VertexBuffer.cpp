#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(ID3D11Device* device, const void* data, unsigned int dataSize) {
	D3D11_BUFFER_DESC bufferDesc;

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = dataSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &pVBuffer);

	if (FAILED(hr)) {
		//TODO: Determine what to do with errors and how to print them
		//For now dump to cout
		std::cout << "VertexBuffer Failed." << std::endl;
	}
}

VertexBuffer::~VertexBuffer() {
	pVBuffer->Release();
}

void VertexBuffer::Bind(ID3D11DeviceContext* deviceContext, UINT stride, UINT offset) {
	deviceContext->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
}