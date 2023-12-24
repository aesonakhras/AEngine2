#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(ID3D11Device* device, const void* data, unsigned int dataSize) {
    D3D11_BUFFER_DESC bufferDesc;

    //TODO: Read into how much I should do this
    //ZeroMemory(&bufferDesc, sizeof(bufferDesc));

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = dataSize;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = data;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    HRESULT hr = device->CreateBuffer(&bufferDesc, &InitData, &pIBuffer);

    if (FAILED(hr)) {
        //TODO: Determine what to do with errors and how to print them
        //For now dump to cout
        std::cout << "VertexBuffer Failed." << std::endl;
    }
}

 void IndexBuffer::Bind(ID3D11DeviceContext* deviceContext, DXGI_FORMAT format, UINT offset) {
	 deviceContext->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);
}

IndexBuffer::~IndexBuffer() {
	pIBuffer->Release();
}