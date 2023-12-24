#include "VertexShader.h"
#include "d3dcompiler.h"
#include <iostream>

VertexShader::VertexShader(ID3D11Device* device, LPCWSTR pFileName) {
	//TODO: IDK what the compiler is yappin about, this is initialized
	pVS = NULL;
	
	ID3DBlob* pErrorBlob = NULL;

	UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG;
	#endif

		HRESULT hr = D3DCompileFromFile(pFileName, 0, 0, "VShader", "vs_4_0", compileFlags, 0, &VS, &pErrorBlob);

		if (FAILED(hr)) {
			std::cout << "Vertex Shader failed" << std::endl;
			if (pErrorBlob != NULL) {
				OutputDebugStringA((LPCSTR)pErrorBlob->GetBufferPointer());
			}
		}

		hr = device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
		if (FAILED(hr)) {
			std::cout << "Vertex Shader failed" << std::endl;
		}
}
void VertexShader::Bind(ID3D11DeviceContext* deviceContext) {
	deviceContext->VSSetShader(pVS, 0, 0);
}

VertexShader::~VertexShader() {
	pVS->Release();
}

ID3DBlob* VertexShader::GetBlob() {
	return VS;
}