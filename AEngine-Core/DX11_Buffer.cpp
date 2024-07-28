#include "DX11_Buffer.h"
#include "Debug.h"
#include <cassert>

using Microsoft::WRL::ComPtr;

DX11_Buffer::DX11_Buffer(ComPtr<ID3D11Device> device, ComPtr <ID3D11DeviceContext> context, size_t size, size_t stride, const void* data, BufferType bufferType) :
    IBuffer( size ), m_device(device), m_deviceContext(context), m_bufferType(ConvertToDX11Buffer(bufferType)), m_stride(stride) {
    
    D3D11_BUFFER_DESC bufferDesc;

    //TODO: Read into how much I should do this
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = size * stride;
    bufferDesc.BindFlags = m_bufferType;
    bufferDesc.CPUAccessFlags = 0;
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
    case AE_Index:
        return D3D11_BIND_INDEX_BUFFER;
        break;
    case AE_Vertex:
        return D3D11_BIND_VERTEX_BUFFER;
        break;
    case AE_Uniform:
        return D3D11_BIND_CONSTANT_BUFFER;
        break;
    default:
        break;
    }

    assert(false && "Invalid buffer provided");
    
    //TODO: Not sure what to return here
    return D3D11_BIND_INDEX_BUFFER;
}