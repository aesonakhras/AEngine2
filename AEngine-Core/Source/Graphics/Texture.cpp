#include "Texture.h"
#include <iostream>

using namespace AE::Graphics;

Texture::Texture(std::shared_ptr<AE::Graphics::IShaderResourceView> _shaderResourceView,
				std::shared_ptr<AE::Graphics::ITextureResource> _textureResource): shaderResourceView(_shaderResourceView),
																						textureResource(_textureResource){
	

}

void Texture::BindAsRenderTarget(unsigned int slot) {
	shaderResourceView->BindAsRenderTarget();
}

void Texture::UnBindRenderTarget(unsigned int slot) {
	shaderResourceView->UnBindAsRenderTarget();
}

void Texture::Bind(unsigned int slot) {
	shaderResourceView->Bind(slot);
}

void Texture::UnBind(unsigned int slot) {
	shaderResourceView->Unbind(slot);
}

void Texture::ClearRenderTarget() {
	shaderResourceView->ClearRenderTarget();
}

Texture::~Texture() {

}