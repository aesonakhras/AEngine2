#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(ID3D11DeviceContext* deviceContext, ID3D11Device* device, const void* data, unsigned int dataSize) :
m_deviceContext(deviceContext),
m_indeciesCount(dataSize) {
    D3D11_BUFFER_DESC bufferDesc;

    //TODO: Read into how much I should do this
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = dataSize;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = sizeof(unsigned int);

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = data;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    HRESULT hr = device->CreateBuffer(&bufferDesc, &InitData, &m_pIBuffer);

    if (FAILED(hr)) {
        //TODO: Determine what to do with errors and how to print them
        //For now dump to cout
        std::cout << "Index buffer Failed." << std::endl;
    }
}

 void IndexBuffer::Bind(DXGI_FORMAT format, UINT offset) {
	 m_deviceContext->IASetIndexBuffer(m_pIBuffer, DXGI_FORMAT_R32_UINT, 0);
}

 unsigned int IndexBuffer::GetSize() {
     //TODO: nah
     return m_indeciesCount / sizeof(unsigned int);
 }

IndexBuffer::~IndexBuffer() {
	m_pIBuffer->Release();
}