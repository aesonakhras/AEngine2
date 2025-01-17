#include "DX11Buffer.h"
#include "Core/Debug.h"
#include <cassert>

using Microsoft::WRL::ComPtr;
using namespace AE::Graphics;

DX11Buffer::DX11Buffer(ComPtr<ID3D11Device> device,
    ComPtr <ID3D11DeviceContext> context,
    size_t size,
    size_t stride,
    const void* data, 
    BufferType bufferType) :
    IBuffer( size ), m_device(device), m_deviceContext(context), m_bufferType(ConvertToDX11Buffer(bufferType)), m_stride(stride) {
    
    D3D11_BUFFER_DESC bufferDesc {};

    bufferDesc.BindFlags = m_bufferType;
    if (m_bufferType & D3D11_BIND_CONSTANT_BUFFER) {
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.ByteWidth = size;
    }
    else {
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.ByteWidth = size * stride;
    }
    
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = stride;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = data;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    HRESULT hr = device->CreateBuffer(&bufferDesc, &InitData, &m_Resource);

    if (FAILED(hr)) {
        AE::Core::Debug::LogError("Buffer creation failed.");
    }    
}

void DX11Buffer::Bind(int slot) const {
    switch (m_bufferType)
    {
    case D3D11_BIND_INDEX_BUFFER:
        m_deviceContext->IASetIndexBuffer(m_Resource.Get(), DXGI_FORMAT_R32_UINT, 0);
        break;
    case D3D11_BIND_VERTEX_BUFFER:
        m_deviceContext->IASetVertexBuffers(slot, 1, m_Resource.GetAddressOf(), &m_stride, &m_offset);
        break;
    case D3D11_BIND_CONSTANT_BUFFER:
        m_deviceContext->VSSetConstantBuffers(slot, 1, m_Resource.GetAddressOf());
        m_deviceContext->PSSetConstantBuffers(slot, 1, m_Resource.GetAddressOf());
        break;
    default:
        break;
    }
}

void DX11Buffer::UnBind() const {
    //Not required for DX11
}

D3D11_BIND_FLAG DX11Buffer::ConvertToDX11Buffer(BufferType bufferType) {
    D3D11_BIND_FLAG d3dBufferFlag;
    
    switch (bufferType)
    {
    case BufferType::Index:
        return D3D11_BIND_INDEX_BUFFER;
        break;
    case BufferType::Vertex:
        return D3D11_BIND_VERTEX_BUFFER;
        break;
    case BufferType::Uniform:
        return D3D11_BIND_CONSTANT_BUFFER;
        break;
    default:
        break;
    }

    assert(false && "Invalid buffer provided");
    
    //TODO: Not sure what to return here
    return D3D11_BIND_INDEX_BUFFER;
}

//TODO: Probably bye bye
void DX11Buffer::Update(const void* data, size_t size) {
    HRESULT hr;
    D3D11_MAPPED_SUBRESOURCE ms;

    hr = m_deviceContext->Map(m_Resource.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
    assert(!FAILED(hr) && "Unable to map resource");
    memcpy(ms.pData, data, size);
    m_deviceContext->Unmap(m_Resource.Get(), NULL);
}

//TODO: This will replace update later
void DX11Buffer::Update2(const void* data, size_t offset, size_t size) {
    HRESULT hr;
    D3D11_MAPPED_SUBRESOURCE ms;

    hr = m_deviceContext->Map(m_Resource.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &ms);

    memcpy(static_cast<char*>(ms.pData) + offset, data, size);
    m_deviceContext->Unmap(m_Resource.Get(), 0);
}