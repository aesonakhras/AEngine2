#pragma once
#include <d3d11.h>

//Why did I call it fragment?
class FragmentShader {
public:
	FragmentShader(ID3D11Device* device, LPCWSTR pFileName);
	void Bind(ID3D11DeviceContext* deviceContext);
	~FragmentShader();
private:
	ID3D11PixelShader* pPS = nullptr;
};