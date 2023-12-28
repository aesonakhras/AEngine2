#pragma once
#include <d3d11.h>

class Texture {
public:
	Texture(ID3D11Device* device, void* data, unsigned int width, unsigned int height, unsigned int miplevel,
		DXGI_FORMAT format, unsigned int sampleCount, unsigned int BindFlags);

	ID3D11Texture2D* GetTexture();

	~Texture();

private:
	ID3D11Texture2D* m_texture = nullptr;
};