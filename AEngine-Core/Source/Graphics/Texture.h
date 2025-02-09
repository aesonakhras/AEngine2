#pragma once
#include "TextureCreateInfo.h"
#include "IShaderResourceView.h"
#include "ITextureResource.h"

namespace AE::Graphics {
	class Texture {
	public:
		Texture(std::shared_ptr<AE::Graphics::IShaderResourceView> _shaderResourceView,
			std::shared_ptr<AE::Graphics::ITextureResource> _textureResource);
		
		void Bind(unsigned int slot);
		void UnBind(unsigned int slot);

		void BindAsRenderTarget(unsigned int slot);
		void UnBindRenderTarget(unsigned int slot);
		
		void ClearRenderTarget();
		~Texture();

	private:
		std::shared_ptr<AE::Graphics::IShaderResourceView> shaderResourceView;
		std::shared_ptr<AE::Graphics::ITextureResource> textureResource;
	};
}