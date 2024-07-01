#include "IndexBuffer.h"

DX11_IndexBuffer::DX11_IndexBuffer(ID3D11DeviceContext* deviceContext, ID3D11Device* device, const void* data, size_t dataSize) :
    IIndexBuffer { dataSize / sizeof(unsigned int),  dataSize} {

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

    HRESULT hr = device->CreateBuffer(&bufferDesc, &InitData, &Resource);

    if (FAILED(hr)) {
        //TODO: Determine what to do with errors and how to print them
        //For now dump to cout
        std::cout << "Index buffer Failed." << std::endl;
    }
}

 unsigned int DX11_IndexBuffer::GetSize() {
     return IndicesCount;
 }

 DX11_IndexBuffer::~DX11_IndexBuffer() {
     Resource->Release();
}