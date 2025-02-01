#include <string>
#include "DX11TextureResource.h"

using namespace AE::Graphics;

DX11TextureResource::DX11TextureResource(
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	Microsoft::WRL::ComPtr<ID3D11Device> device,
	const TextureCreateInfo& createInfo
):
Type(createInfo.type),
Format(createInfo.format) 
{
	D3D11_TEXTURE2D_DESC desc = CreateTextureDescription(createInfo);

	HRESULT hr = S_OK;

	if (createInfo.data == nullptr) {
		// Create an empty texture for depth/stencil
		hr = device->CreateTexture2D(&desc, nullptr, m_texture.GetAddressOf());
	}
	else {
		// Generate subresource data
		std::vector<D3D11_SUBRESOURCE_DATA> subresourceData = GenerateSubresourceData(createInfo);

		if (createInfo.generateMipMaps) {
			// Create an empty texture then upload first mip
			hr = device->CreateTexture2D(&desc, nullptr, m_texture.GetAddressOf());
			if (SUCCEEDED(hr)) {
				UpdateBaseLevel(deviceContext, createInfo);
			}
		}
		else {
			// Create texture with provided subresources
			hr = device->CreateTexture2D(&desc, subresourceData.data(), m_texture.GetAddressOf());
		}
	}

	if (FAILED(hr)) {
		AE::Core::Debug::LogError("Unable to create DX11 Texture resource");
		throw std::runtime_error("Failed to create texture.");
	}
}

D3D11_TEXTURE2D_DESC DX11TextureResource::CreateTextureDescription(const TextureCreateInfo& createInfo) {
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = createInfo.width;
	desc.Height = createInfo.height;
	desc.MipLevels = createInfo.mipLevels;
	desc.ArraySize = (Type == TextureType::Cubemap) ? 6 : 1;
	desc.Format = ConvertToDX11Format(Format);
	desc.SampleDesc.Count = 1; // No multisampling for now
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = ConvertToDX11TextureBinding(createInfo.use);
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	
	if (Type == TextureType::Cubemap) {
		desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
	}

	if (createInfo.generateMipMaps) {
		desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		desc.MipLevels = 0; //set to zero to signal generate map
	}

	return desc;
}

std::vector<D3D11_SUBRESOURCE_DATA> DX11TextureResource::GenerateSubresourceData(const TextureCreateInfo& createInfo) {
	std::vector<D3D11_SUBRESOURCE_DATA> subresourceData;

	if (Type == TextureType::Cubemap) {
		for (uint32 face = 0; face < 6; face++) {
			for (uint32 mip = 0; mip < createInfo.mipLevels; mip++) {
				AddSubresourceData(subresourceData, createInfo, face, mip);
			}
		}
	}
	else {
		for (uint32 mip = 0; mip < createInfo.mipLevels; mip++) {
			AddSubresourceData(subresourceData, createInfo, 0, mip);
		}
	}

	return subresourceData;
}

void DX11TextureResource::AddSubresourceData(
	std::vector<D3D11_SUBRESOURCE_DATA>& subresourceData,
	const TextureCreateInfo& createInfo,
	uint32 face,
	uint32 mip
) {
	uint32 mipWidth = createInfo.width >> mip;
	uint32 mipHeight = createInfo.height >> mip;

	if (mipWidth == 0) mipWidth = 1;
	if (mipHeight == 0) mipHeight = 1;

	uint32 pitch = 0;

	if (createInfo.format == TextureFormat::BC7_UNORM) {
		pitch = ((mipWidth + 3) / 4) * 16;
	}
	else {
		pitch = mipWidth * 4;
	}

	D3D11_SUBRESOURCE_DATA initData{};
	
	//uint32 mipOffset = createInfo.imageOffsets[mip];
	//uint32 faceOffset = pitch * mipHeight;

	uint32 offset = createInfo.imageOffsets[face * createInfo.mipLevels + mip];

	initData.pSysMem = createInfo.data.get() + offset;
	initData.SysMemPitch = pitch;
	initData.SysMemSlicePitch = 0;

	subresourceData.push_back(initData);
}

void DX11TextureResource::UpdateBaseLevel(
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	const TextureCreateInfo& createInfo
) {
	deviceContext->UpdateSubresource(
		m_texture.Get(),
		0,
		nullptr,
		createInfo.data.get(),
		createInfo.width * 4, //assumes rgba8
		0
	);
}

void* DX11TextureResource::Get() const {
	return m_texture.Get();
}