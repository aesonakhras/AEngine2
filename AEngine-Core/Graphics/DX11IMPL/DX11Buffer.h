#pragma once

//TODO: This should be removed if possible:
#include <iostream>
#include <d3d11.h>
#include <wrl/client.h>
#include "../Graphics/IBuffer.h"


namespace AE::Graphics {
	class DX11Buffer : public IBuffer {
	public:
		DX11Buffer(Microsoft::WRL::ComPtr<ID3D11Device> device,
			Microsoft::WRL::ComPtr <ID3D11DeviceContext> context,
			size_t size,
			size_t stride,
			const void* data,
			BufferType bufferType
		);
		virtual void Bind() const final;
		virtual void UnBind() const final;
		virtual void Update(const void* data, size_t size) final;
		~DX11Buffer() final { };

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Resource;

		D3D11_BIND_FLAG m_bufferType;

		D3D11_BIND_FLAG ConvertToDX11Buffer(BufferType bufferType);

		UINT m_stride;
		UINT m_offset = 0;
	};
}