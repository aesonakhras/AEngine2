#pragma once
#include <d3d11.h>
#include <vector>
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
			D3D11_TEXTURE2D_DESC CreateTextureDescription(const TextureCreateInfo& createInfo);
			
			std::vector<D3D11_SUBRESOURCE_DATA> GenerateSubresourceData(const TextureCreateInfo& createInfo);
			
			void AddSubresourceData(
				std::vector<D3D11_SUBRESOURCE_DATA>& subresourceData,
				const TextureCreateInfo& createInfo,
				uint32 face,
				uint32 mip
			);

			void UpdateBaseLevel(
				Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
				const TextureCreateInfo& createInfo
			);

			Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
	};
}