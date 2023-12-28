#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(ID3D11DeviceContext* deviceContext, ID3D11Device* device, const void* data, unsigned int dataSize) :
m_deviceContext(deviceContext) {
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

	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &m_pVBuffer);

	if (FAILED(hr)) {
		//TODO: Determine what to do with errors and how to print them
		//For now dump to cout
		std::cout << "VertexBuffer Failed." << std::endl;
	}
}

VertexBuffer::~VertexBuffer() {
	m_pVBuffer->Release();
}

void VertexBuffer::Bind(UINT stride, UINT offset) {
	m_deviceContext->IASetVertexBuffers(0, 1, &m_pVBuffer, &stride, &offset);
}