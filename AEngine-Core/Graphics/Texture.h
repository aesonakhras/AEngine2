#pragma once
#include "../Graphics/TextureCreateInfo.h"
#include "../System/Memory/AEMemory.h"
#include "../Graphics/IShaderResourceView.h"
#include "../Graphics/ITextureResource.h"

//TODO: put this in a proper namespace

namespace AE::Graphics {
	class Texture {
	public:
		Texture(std::shared_ptr<AE::Graphics::IShaderResourceView> _shaderResourceView,
			std::shared_ptr<AE::Graphics::ITextureResource> _textureResource);
		void Bind(unsigned int slot);
		~Texture();

	private:
		std::shared_ptr<AE::Graphics::IShaderResourceView> shaderResourceView;
		std::shared_ptr<AE::Graphics::ITextureResource> textureResource;
	};
}