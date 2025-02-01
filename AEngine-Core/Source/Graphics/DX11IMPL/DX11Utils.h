#pragma once


//TODO: I REALLY don't like this class, try to get rid of it soon
//probably integrate it with the GLI
#include <d3d11.h>

#include "Graphics/TextureFormat.h"
#include "Graphics/TextureBindFlag.h"
#include "Graphics/TextureCreateInfo.h"
#include "Core/Debug.h"

namespace AE::Graphics {
	//TODO: Consider a dx11 namespace, also make sure the converted formats make actual sense for later on
	inline DXGI_FORMAT ConvertToDX11Format(TextureFormat format) {
		switch (format)
		{
		case TextureFormat::R8U :
			return DXGI_FORMAT_R8_UINT;
			break;
		case TextureFormat::RGBA8U:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case TextureFormat::RG32F:
			return DXGI_FORMAT_R32G32_FLOAT;
			break;
		case TextureFormat::RGB32F:
			return DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case TextureFormat::RGBA32F:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		case TextureFormat::BC7_UNORM:
			return DXGI_FORMAT_BC7_UNORM;
			break;
		case TextureFormat::R32_TYPELESS:
			return DXGI_FORMAT_R32_TYPELESS;
			break;
		case TextureFormat::R24G8_TYPELESS:
			return DXGI_FORMAT_R24G8_TYPELESS;
			break;
		default:
			AE::Core::Debug::LogWarning("Unsupported texture format used");
			break;
		}
	}

	//Does not prevent invalid combinations assumes you know what you are doing
	//inline UINT ConvertToDX11TextureBinding(TextureBindFlag usage) {
	//	UINT result = 0;

	//	if (usage & TextureBindFlag::ShaderResource ) {
	//		result |= D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	//	}

	//	if (usage & TextureBindFlag::RenderTarget) {
	//		result |= D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
	//	}

	//	if (usage & TextureBindFlag::DepthStencil) {
	//		result |= D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	//	}
	//	
	//	return result;
	//}

	inline UINT ConvertToDX11TextureBinding(TextureUse use) {
		UINT result = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;

		if (use == TextureUse::RenderTexture) {
			result |= D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
		}
		else if (use == TextureUse::DepthTexture) {
			result |= D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		}

		return result;
	}

	inline D3D11_SRV_DIMENSION ConvertTODX11SRVDimension(TextureType type) {
		if (type == TextureType::Cubemap) {
			return D3D11_SRV_DIMENSION_TEXTURECUBE;
		}

		return D3D11_SRV_DIMENSION_TEXTURE2D;
	}
}