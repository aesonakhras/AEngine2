#include "VertexShader.h"
#include "d3dcompiler.h"

#include <iostream>

//Putting this here for now, will refactor l8r
struct VS_CONSTANT_BUFFER
{
	DirectX::XMMATRIX mWorldViewProj;
	DirectX::XMFLOAT4 cb_color;
};

void VertexShader::setupConstantbuffer(ID3D11Device* device, DirectX::XMFLOAT4 color) {

	VS_CONSTANT_BUFFER VsConstData{ DirectX::XMMatrixIdentity(), color };

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &VsConstData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&cbDesc, &InitData,
		&m_pConstantBuffer);

	if (FAILED(hr)) {
		std::cout << "Constant Buffer Failed" << std::endl;
	}
}
/////////////////////////////////

VertexShader::VertexShader(ID3D11DeviceContext* deviceContext, ID3D11Device* device, LPCWSTR pFileName, DirectX::XMFLOAT4 color, VextexLayout layout) :
	m_deviceContext (deviceContext),
	m_color(color) {
	//TODO: IDK what the compiler is yappin about, this is initialized
	m_pVS = NULL;
	
	ID3DBlob* pErrorBlob = NULL;

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


	//set up the layout
	device->CreateInputLayout(m_ied, 2, m_VS->GetBufferPointer(),
		m_VS->GetBufferSize(), &m_layout);

	setupConstantbuffer(device, color);
}
void VertexShader::Bind() {
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_deviceContext->IASetInputLayout(m_layout);
	m_deviceContext->VSSetShader(m_pVS, 0, 0);
}

void VertexShader::SetMVP(DirectX::XMMATRIX mvp) {
	auto transposeMatrix = DirectX::XMMatrixTranspose(mvp);

	VS_CONSTANT_BUFFER VsConstData{ transposeMatrix, m_color };

	//update stuff will happen here
	D3D11_MAPPED_SUBRESOURCE ms;
	m_deviceContext->Map(m_pConstantBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
	memcpy(ms.pData, &VsConstData, sizeof(VS_CONSTANT_BUFFER));                // copy the data
	m_deviceContext->Unmap(m_pConstantBuffer, NULL);                          // unmap the buffer
}

VertexShader::~VertexShader() {
	m_pVS->Release();
}