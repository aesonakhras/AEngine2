#pragma once
#include <d3d11.h>

//Why did I call it fragment?
class FragmentShader {
public:
	FragmentShader(ID3D11DeviceContext* deviceContext, ID3D11Device* device, LPCWSTR pFileName);
	void Bind();
	~FragmentShader();
private:
	ID3D11PixelShader* m_pPS = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
};