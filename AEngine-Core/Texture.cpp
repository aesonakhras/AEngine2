#include "Texture.h"
#include <iostream>

Texture::Texture(Microsoft::WRL::ComPtr <ID3D11Device> device, void* data, unsigned int width, unsigned int height, unsigned int miplevel,
	DXGI_FORMAT format, unsigned int sampleCount, unsigned int BindFlags) {
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = miplevel;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = sampleCount;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = BindFlags; //change
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = S_OK;

	if (data == nullptr) {
		//for depth/stencil buffer
		hr = device->CreateTexture2D(&desc, NULL, &m_texture);
	}
	else {
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;
		initData.SysMemPitch = width * 4;
		initData.SysMemSlicePitch = 0;

		hr = device->CreateTexture2D(&desc, &initData, &m_texture);
	}

	if (FAILED(hr)) {
		std::cout << "Texture Creation Fail" << std::endl;
	}
}

ID3D11Texture2D* Texture::GetTexture() {
	return m_texture;
}

Texture::~Texture() {
	m_texture->Release();
}