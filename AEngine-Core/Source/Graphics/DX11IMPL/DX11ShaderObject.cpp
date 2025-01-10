#include "d3dcompiler.h"

#include "DX11ShaderObject.h"
#include <Windows.h>

using AE::Core::Debug;
using namespace AE::Graphics;

DX11ShaderObject::DX11ShaderObject(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr<ID3D11Device> device, const void* data, size_t dataSize, std::string entryPoint, std::string shaderTarget) :
	m_deviceContext(deviceContext), m_device(device) {

	//TODO: May wnt to defer this to compile to have control
	Compile(
		data,
		dataSize,
		entryPoint,
		shaderTarget
	);
}

bool DX11ShaderObject::Compile(
	const void* data,
	size_t dataSize,
	std::string entryPoint,
	std::string shaderTarget
) {
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

		return false;
	}

	return true;
}

Microsoft::WRL::ComPtr <ID3D10Blob> DX11ShaderObject::GetShaderBlob() {
	return m_shaderBlob;
}