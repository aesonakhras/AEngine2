#pragma once
#include "Graphics/TextureCreateInfo.h"
#include "../System/Memory/AEMemory.h"
#include "Graphics/ISampler.h"
#include "Graphics/IShaderResourceView.h"
#include "Graphics/ITextureResource.h"


class Texture {
public:
	Texture(std::shared_ptr<AE::Core::Graphics::IShaderResourceView> _shaderResourceView,
			std::shared_ptr<AE::Core::Graphics::ISampler> _sampler,
			std::shared_ptr<AE::Core::Graphics::ITextureResource> _textureResource);
	void Bind();
	~Texture();

private:
	//TODO: This is absurdly nasty, flatten namespace structure and enforce better naming standards
	std::shared_ptr<AE::Core::Graphics::IShaderResourceView> shaderResourceView;
	std::shared_ptr<AE::Core::Graphics::ISampler> sampler;
	std::shared_ptr<AE::Core::Graphics::ITextureResource> textureResource;
};