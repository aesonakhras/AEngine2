#include "DX11TextureResource.h"

using namespace AE::Core::Graphics;

DX11TextureResource::DX11TextureResource(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr<ID3D11Device> device, const TextureCreateInfo& createInfo) {
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width = createInfo.width;
	desc.Height = createInfo.height;
	desc.MipLevels = createInfo.mipLevels;
	desc.ArraySize = 1;
	desc.Format = ConvertToDX11Format(createInfo.format);
	desc.SampleDesc.Count = createInfo.sampleCount;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = ConvertToDX11TextureBinding(createInfo.bindFlags); //change
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = S_OK;

	if (createInfo.data == nullptr) {
		//for depth/stencil buffer
		hr = device->CreateTexture2D(&desc, NULL, m_texture.GetAddressOf());
	}
	else {
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = createInfo.data;
		initData.SysMemPitch = createInfo.width * 4;
		initData.SysMemSlicePitch = 0;

		hr = device->CreateTexture2D(&desc, &initData, m_texture.GetAddressOf());
	}

	if (FAILED(hr)) {
		AECore::Debug::LogError("Unable to create DX11 Texture resource");
	}
}

void* DX11TextureResource::Get() const {
	return m_texture.Get();
}