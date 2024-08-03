#include "../DX11_Shader.h"

using AECore::Debug;

DX11_Shader::DX11_Shader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	Microsoft::WRL::ComPtr <ID3D11Device> device, LPCWSTR pFileName, AEngine::Graphics::ShaderType shaderType) : m_shaderType(shaderType) {
	
	Microsoft::WRL::ComPtr <ID3DBlob> pErrorBlob = NULL;

	UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG;
	#endif

	HRESULT hr = D3DCompileFromFile(pFileName, 0, 0, "VShader", "vs_4_0", compileFlags, 0, &m_shaderBlob, &pErrorBlob);

	if (FAILED(hr)) {
		Debug::LogError("Shader Creation Failed");
		if (pErrorBlob != NULL) {
			OutputDebugStringA((LPCSTR)pErrorBlob->GetBufferPointer());
		}
	}

	switch (m_shaderType)
	{
	case AEngine::Graphics::ShaderType::Vertex:
		hr = device->CreateVertexShader(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), NULL, &m_shader.VS);
		break;
	case AEngine::Graphics::ShaderType::Fragment:
		hr = device->CreatePixelShader(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), NULL, &m_shader.FS);
		break;
	default:
		break;
	}

	if (FAILED(hr)) {
		Debug::LogError("Shader Creation Failed");
	}
}

void DX11_Shader::Bind() {

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

void DX11_Shader::UnBind() {
	//TODO: Is this necessary??
}

void DX11_Shader() {
	//clean up the shader since it is contained in a union
}