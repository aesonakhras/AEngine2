#include "../DX11_Shader.h"
#include <Windows.h>

using AECore::Debug;
using namespace AE::Core::System;

DX11_Shader::DX11_Shader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	Microsoft::WRL::ComPtr <ID3D11Device> device, std::string pFileName, AEngine::Graphics::ShaderType shaderType) : m_shaderType(shaderType) {
	
	Microsoft::WRL::ComPtr <ID3DBlob> pErrorBlob = NULL;

	UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG;
	#endif
	HRESULT hr = S_OK;

	//Load File
	FileManager& fileManager = FileManager::GetInstance();

	auto fileHandle = fileManager.GetFile(pFileName, FileOperation::Read);

	auto data = fileHandle->ReadAll();

	D3D_SHADER_MACRO defines[] = {
	{ nullptr, nullptr }        //put macros here if there are any
	};
	
	switch (m_shaderType)
	{
	case AEngine::Graphics::ShaderType::Vertex:


		

		hr = D3DCompile2(data.data(), 
						data.size(), 
						nullptr, 
						defines,
						nullptr,
						"VShader",
						"vs_4_0", 
						compileFlags,
						0,
						0,
						nullptr,
						0,
						&m_shaderBlob,
						&pErrorBlob);

		//hr = D3DCompileFromFile(data.data(), 0, 0, "VShader", "vs_4_0", compileFlags, 0, &m_shaderBlob, &pErrorBlob);
		if (SUCCEEDED(hr)) {
			hr = device->CreateVertexShader(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), NULL, &m_shader.VS);
		}
		break;
	case AEngine::Graphics::ShaderType::Fragment:
		hr = D3DCompile2(data.data(),
			data.size(),
			nullptr,
			defines,
			nullptr,
			"PShader",
			"ps_4_0",
			compileFlags,
			0,
			0,
			nullptr,
			0,
			&m_shaderBlob,
			&pErrorBlob);

		//hr = D3DCompileFromFile(pFileName, 0, 0, "PShader", "ps_4_0", compileFlags, 0, &m_shaderBlob, &pErrorBlob);
		if (SUCCEEDED(hr)) {
			hr = device->CreatePixelShader(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), NULL, &m_shader.FS);
		}
		break;
	default:
		break;
	}

	if (FAILED(hr)) {
		Debug::LogError("Shader Creation Failed");

		if (pErrorBlob != NULL) {
			OutputDebugStringA((LPCSTR)pErrorBlob->GetBufferPointer());
		}
	}
}

void DX11_Shader::Bind() const {

	switch (m_shaderType)
	{
	case AEngine::Graphics::ShaderType::Vertex:
		m_deviceContext->VSSetShader(m_shader.VS.Get(), 0, 0);
		break;
	case AEngine::Graphics::ShaderType::Fragment:
		m_deviceContext->PSSetShader(m_shader.FS.Get(), 0, 0);
		break;
	default:
		break;
	}
}

void DX11_Shader::UnBind() const {
	//TODO: Is this necessary??
}

void DX11_Shader() {
	//clean up the shader since it is contained in a union
}