#pragma once
#include <d3d11.h>
#include <wrl/client.h>

#include "../ITextureResource.h"
#include "../TextureCreateInfo.h"
#include "../Debug.h"
#include "../DX11IMPL/DX11Utils.h"

namespace AE::Core::Graphics {
	class DX11TextureResource : public ITextureResource {
		public:

			DX11TextureResource(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr<ID3D11Device> device, const TextureCreateInfo& createInfo);

			virtual void* Get() const override final;

		private:
			Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
	};
}