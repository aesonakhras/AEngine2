#include "Texture.h"
#include <iostream>

using namespace AE::Graphics;

Texture::Texture(std::shared_ptr<AE::Graphics::IShaderResourceView> _shaderResourceView,
				std::shared_ptr<AE::Graphics::ITextureResource> _textureResource): shaderResourceView(_shaderResourceView),
																						textureResource(_textureResource){
	

}

void Texture::Bind(unsigned int slot) {
	shaderResourceView->Bind(slot);
}

Texture::~Texture() {

}