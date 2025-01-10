#include <d3d11.h>

#include "Graphics/VertexAttribute.h"

#include "DX11VertexShader.h"
#include "DX11ShaderObject.h"

using namespace AE::Graphics;

DX11VertexShader::DX11VertexShader(
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	Microsoft::WRL::ComPtr<ID3D11Device> device,
	std::shared_ptr<DX11ShaderObject> shader,
	const std::vector<VertexAttribute>& attribs
) :
	m_deviceContext(deviceContext),
	m_device(device),
	Shader(shader),
	m_layout(deviceContext, device)
{
	auto blob = Shader->GetShaderBlob();

	HRESULT hr = m_device->CreateVertexShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		NULL,
		m_vertexShader.GetAddressOf());

	m_layout.Build(attribs, Shader->GetShaderBlob());
}

bool DX11VertexShader::Recreate() {	
	auto blob = Shader->GetShaderBlob();

	HRESULT hr = m_device->CreateVertexShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		NULL,
		m_vertexShader.GetAddressOf());

	m_layout.Rebuild(Shader->GetShaderBlob());

	return true;
}

void DX11VertexShader::Bind() {
	m_layout.Bind();
	m_deviceContext->VSSetShader(m_vertexShader.Get(), 0, 0);
};

void DX11VertexShader::UnBind() {

};