#pragma once

//TODO: This should be removed if possible:
#include <iostream>
#include <d3d11.h>
#include <wrl/client.h>
#include "IBuffer.h"

using Microsoft::WRL::ComPtr;

class DX11_Buffer : public IBuffer {
public:
	DX11_Buffer(ComPtr<ID3D11Device> device, ComPtr <ID3D11DeviceContext> context, size_t size, size_t stride, const void* data, BufferType bufferType);
	virtual void Bind() const final;
	virtual void UnBind() const final;
	~DX11_Buffer() final { };

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11Buffer> m_Resource;

	D3D11_BIND_FLAG m_bufferType;

	D3D11_BIND_FLAG ConvertToDX11Buffer(BufferType bufferType);

	UINT m_stride;
	UINT m_offset = 0;
};