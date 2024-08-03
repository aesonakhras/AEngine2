#include "VertexShader.h"
#include "d3dcompiler.h"

#include <iostream>



//Putting this here for now, will refactor l8r
struct VS_CONSTANT_BUFFER
{
	DirectX::XMMATRIX mWorldViewProj;
	DirectX::XMFLOAT4 cb_color;
};
/////////////////////////////////

VertexShader::VertexShader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, 
	Microsoft::WRL::ComPtr<ID3D11Device> device, LPCWSTR pFileName, DirectX::XMFLOAT4 color, VextexLayout layout) :
	m_deviceContext (deviceContext),
	m_color(color) {	
	Microsoft::WRL::ComPtr <ID3DBlob> pErrorBlob = NULL;

	UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG;
	#endif

	HRESULT hr = D3DCompileFromFile(pFileName, 0, 0, "VShader", "vs_4_0", compileFlags, 0, &m_VS, &pErrorBlob);

	if (FAILED(hr)) {
		std::cout << "Vertex Shader failed" << std::endl;
		if (pErrorBlob != NULL) {
			OutputDebugStringA((LPCSTR)pErrorBlob->GetBufferPointer());
		}
	}

	hr = device->CreateVertexShader(m_VS->GetBufferPointer(), m_VS->GetBufferSize(), NULL, &m_pVS);
	if (FAILED(hr)) {
		std::cout << "Vertex Shader failed" << std::endl;
	}

	m_layout = std::make_shared<DX11_InputLayout<AEngineVertexTypes::VERTEX2>>(deviceContext, device, m_VS );

	VS_CONSTANT_BUFFER VsConstData{ DirectX::XMMatrixIdentity(), color };

	m_constantBuffer = std::make_shared<DX11_Buffer>(device, deviceContext, sizeof(VsConstData), 1, &VsConstData, AEngine::Graphics::BufferType::Uniform);
}

void VertexShader::Bind() {
	m_constantBuffer->Bind();
	m_layout->Bind();
	m_deviceContext->VSSetShader(m_pVS.Get(), 0, 0);
}

void VertexShader::SetMVP(DirectX::XMMATRIX mvp) {
	auto transposeMatrix = DirectX::XMMatrixTranspose(mvp);

	VS_CONSTANT_BUFFER VsConstData{ transposeMatrix, m_color };
	m_constantBuffer->Update(&VsConstData, sizeof(VS_CONSTANT_BUFFER));
}

VertexShader::~VertexShader() {
	m_pVS->Release();
}