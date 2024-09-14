#include <d3d11.h>
#include "DX11VertexShader.h"
#include "../Graphics/DX11IMPL/DX11ShaderObject.h"
#include "../Graphics/VertexAttribute.h"

using namespace AE::Graphics;

DX11VertexShader::DX11VertexShader(
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	Microsoft::WRL::ComPtr<ID3D11Device> device,
	std::shared_ptr<DX11ShaderObject> shader,
	const std::vector<VertexAttribute>& attribs
) :
	m_deviceContext(deviceContext),
	m_shader(shader),
	m_layout(deviceContext, device, m_shader->Get<ID3D10Blob>())
{
	ID3D10Blob* blob = m_shader->Get<ID3D10Blob>();

	HRESULT hr = device->CreateVertexShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		NULL,
		m_vertexShader.GetAddressOf());

	m_layout.Build(attribs);
}

void DX11VertexShader::Bind() {
	m_layout.Bind();
	m_deviceContext->VSSetShader(m_vertexShader.Get(), 0, 0);
};

void DX11VertexShader::UnBind() {

};