#include "Texture.h"
#include <iostream>

using namespace AE::Graphics;

Texture::Texture(std::shared_ptr<AE::Graphics::IShaderResourceView> _shaderResourceView,
				std::shared_ptr<AE::Graphics::ISampler> _sampler,
				std::shared_ptr<AE::Graphics::ITextureResource> _textureResource): shaderResourceView(_shaderResourceView),
																						sampler(_sampler),
																						textureResource(_textureResource){
	

}

void Texture::Bind() {
	shaderResourceView->Bind(0);
	sampler->Bind(0);
}

Texture::~Texture() {

}