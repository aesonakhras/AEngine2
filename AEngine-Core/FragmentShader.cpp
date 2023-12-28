#include "FragmentShader.h"
#include "d3dcompiler.h"
#include <iostream>

#include <comdef.h>

FragmentShader::FragmentShader(ID3D11DeviceContext* deviceContext, ID3D11Device* device, LPCWSTR pFileName) :
	m_deviceContext(deviceContext) {
	//TODO: IDK what the compiler is yappin about, this is initialized
	ID3D10Blob* PS = nullptr;

	ID3DBlob* pErrorBlob = nullptr;

	UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG;
	#endif

	HRESULT hr = D3DCompileFromFile(pFileName, 0, 0, "PShader", "ps_4_0", compileFlags, 0, &PS, &pErrorBlob);

	if (FAILED(hr)) {
		std::cout << "Fragment Shader failed" << std::endl;
		if (pErrorBlob != NULL) {
			OutputDebugStringA((LPCSTR)pErrorBlob->GetBufferPointer());
		}
	}

	hr = device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPS);
	if (FAILED(hr)) {
		std::cout << "Fragment Shader failed" << std::endl;
	}
}
void FragmentShader::Bind() {
	m_deviceContext->PSSetShader(m_pPS, 0, 0);
}

FragmentShader::~FragmentShader() {
	m_pPS->Release();
}