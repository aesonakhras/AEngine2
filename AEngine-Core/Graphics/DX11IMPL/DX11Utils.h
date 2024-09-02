#pragma once
#include <d3d11.h>

#include "../Graphics/TextureFormat.h"
#include "../Debug.h"

namespace AE::Core::Graphics {
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
		case TextureFormat::RGBA32F:
			return DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		default:
			AECore::Debug::LogWarning("Unsupported texture format used");
			break;
		}
	}

	//Does not prevent invalid combinations assumes you know what you are doing
	inline UINT ConvertToDX11TextureBinding(TextureBindFlag usage) {
		UINT result = 0;

		if (usage & TextureBindFlag::ShaderResource ) {
			result |= D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		}

		if (usage & TextureBindFlag::RenderTarget) {
			result |= D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
		}

		if (usage & TextureBindFlag::DepthStencil) {
			result |= D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		}
		
		return result;
	}
}