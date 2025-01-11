#pragma once
#include <d3d11.h>
#include <wrl/client.h>

#include "Graphics/ITextureResource.h"
#include "Graphics/TextureCreateInfo.h"
#include "Core/Debug.h"
#include "DX11Utils.h"

namespace AE::Graphics {
	class DX11TextureResource : public ITextureResource {
		public:

			DX11TextureResource(
				Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
				Microsoft::WRL::ComPtr<ID3D11Device> device,
				const TextureCreateInfo& createInfo
			);

			virtual void* Get() const override final;

			TextureType Type {};
			TextureFormat Format {};

		private:
			Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
	};
}