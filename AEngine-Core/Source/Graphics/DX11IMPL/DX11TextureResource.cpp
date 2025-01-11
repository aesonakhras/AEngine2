#include "DX11TextureResource.h"

using namespace AE::Graphics;

DX11TextureResource::DX11TextureResource(
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	Microsoft::WRL::ComPtr<ID3D11Device> device,
	const TextureCreateInfo& createInfo
) {
	D3D11_TEXTURE2D_DESC desc {};
	ZeroMemory(&desc, sizeof(desc));

	Type = createInfo.type;
	Format = createInfo.format;

	desc.Width = createInfo.width;
	desc.Height = createInfo.height;
	desc.MipLevels = createInfo.mipLevels;
	desc.ArraySize = createInfo.arraySize;
	desc.Format = ConvertToDX11Format(Format);
	desc.SampleDesc.Count = createInfo.sampleCount;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = ConvertToDX11TextureBinding(createInfo.bindFlags); //change
	desc.CPUAccessFlags = 0;

	if (Type == TextureType::Cubemap) {
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	}

	HRESULT hr = S_OK;

	if (createInfo.data[0] == nullptr) {
		//for depth/stencil buffer
		hr = device->CreateTexture2D(&desc, NULL, m_texture.GetAddressOf());
	}
	else {
		std::vector<D3D11_SUBRESOURCE_DATA> subresourceData;

		//TODO: this width * 4 will not work if I do anything other than RGBA
		if (Type == TextureType::Cubemap) {
			for (int i = 0; i < 6; i++) {
				D3D11_SUBRESOURCE_DATA initData {};
				initData.pSysMem = createInfo.data[i];
				initData.SysMemPitch = createInfo.width * 4;
				initData.SysMemSlicePitch = 0;

				subresourceData.push_back(initData);
			}
		}
		else {
			D3D11_SUBRESOURCE_DATA initData {};
			initData.pSysMem = createInfo.data[0];
			initData.SysMemPitch = createInfo.width * 4;
			initData.SysMemSlicePitch = 0;

			subresourceData.push_back(initData);
		}

		hr = device->CreateTexture2D(&desc, subresourceData.data(), m_texture.GetAddressOf());
	}

	if (FAILED(hr)) {
		AE::Core::Debug::LogError("Unable to create DX11 Texture resource");
	}
}

void* DX11TextureResource::Get() const {
	return m_texture.Get();
}