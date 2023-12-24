#pragma once
#include <d3d11.h>

class VertexShader {
public:
	VertexShader(ID3D11Device* device, LPCWSTR pFileName);
	void Bind(ID3D11DeviceContext* deviceContext);
	ID3DBlob* GetBlob();
	~VertexShader();
private:
	ID3D11VertexShader* pVS = nullptr;
	ID3D10Blob* VS = nullptr;
};