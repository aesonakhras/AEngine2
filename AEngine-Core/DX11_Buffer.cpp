#include "DX11_Buffer.h"
#include "Debug.h"
#include <cassert>

using Microsoft::WRL::ComPtr;
using namespace AE::Graphics;

DX11_Buffer::DX11_Buffer(ComPtr<ID3D11Device> device,
    ComPtr <ID3D11DeviceContext> context,
    size_t size,
    size_t stride,
    const void* data, 
    BufferType bufferType) :
    IBuffer( size ), m_device(device), m_deviceContext(context), m_bufferType(ConvertToDX11Buffer(bufferType)), m_stride(stride) {
    
    D3D11_BUFFER_DESC bufferDesc;

    
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));

    bufferDesc.BindFlags = m_bufferType;
    if (m_bufferType & D3D11_BIND_CONSTANT_BUFFER) {
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else {
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.CPUAccessFlags = 0;
    }
    bufferDesc.ByteWidth = size * stride;
    
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = stride;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = data;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    HRESULT hr = device->CreateBuffer(&bufferDesc, &InitData, &m_Resource);

    if (FAILED(hr)) {
        AECore::Debug::LogError("Buffer creation failed.");
    }    
}

void DX11_Buffer::Bind() const {
    switch (m_bufferType)
    {
    case D3D11_BIND_INDEX_BUFFER:
        m_deviceContext->IASetIndexBuffer(m_Resource.Get(), DXGI_FORMAT_R32_UINT, 0);
        break;
    case D3D11_BIND_VERTEX_BUFFER:
        m_deviceContext->IASetVertexBuffers(0, 1, m_Resource.GetAddressOf(), &m_stride, &m_offset);
        break;
    case D3D11_BIND_CONSTANT_BUFFER:
        m_deviceContext->VSSetConstantBuffers(0, 1, m_Resource.GetAddressOf());
        break;
    default:
        break;
    }
}

void DX11_Buffer::UnBind() const {
    //Not required for DX11
}

D3D11_BIND_FLAG DX11_Buffer::ConvertToDX11Buffer(BufferType bufferType) {
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

void DX11_Buffer::Update(const void* data, size_t size) {
    HRESULT hr;
    D3D11_MAPPED_SUBRESOURCE ms;

    hr = m_deviceContext->Map(m_Resource.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
    assert(!FAILED(hr) && "Unable to map resource");
    memcpy(ms.pData, data, size);
    m_deviceContext->Unmap(m_Resource.Get(), NULL);
}