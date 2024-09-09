#include "DX11ShaderObject.h"
#include <Windows.h>

using AECore::Debug;
using namespace AE::Core::System;

DX11ShaderObject::DX11ShaderObject(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr<ID3D11Device> device, const void* data, size_t dataSize, std::string entryPoint, std::string shaderTarget) :
	m_deviceContext(deviceContext), m_device(device) {
	Microsoft::WRL::ComPtr <ID3DBlob> pErrorBlob = NULL;

	UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
	compileFlags |= D3DCOMPILE_DEBUG;
#endif

	HRESULT hr = S_OK;

	D3D_SHADER_MACRO defines[] = { { nullptr, nullptr } }; //put macros here if there are any

	hr = D3DCompile2(data,
		dataSize,
		nullptr,
		defines,
		nullptr,
		entryPoint.c_str(),
		shaderTarget.c_str(),
		compileFlags,
		0,
		0,
		nullptr,
		0,
		&m_shaderBlob,
		&pErrorBlob);


	if (FAILED(hr)) {
		Debug::LogError("Shader Creation Failed");

		if (pErrorBlob != NULL) {
			OutputDebugStringA((LPCSTR)pErrorBlob->GetBufferPointer());
		}
	}

	//hr = device->CreateVertexShader(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), NULL, &m_shader.VS);
	//hr = device->CreatePixelShader(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), NULL, &m_shader.FS);
}

void* DX11ShaderObject::GetRawObject() {
	return static_cast<void*>(m_shaderBlob.Get());
}