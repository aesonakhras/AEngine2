#include <d3d11.h>

#include "DX11FragmentShader.h"
#include "DX11ShaderObject.h"

using namespace AE::Graphics;

DX11FragmentShader::DX11FragmentShader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, 
	Microsoft::WRL::ComPtr<ID3D11Device> device, 
	std::shared_ptr<DX11ShaderObject> shader) : 
	m_deviceContext(deviceContext), 
	m_shader(shader) {
	
	ID3D10Blob* blob = m_shader->Get<ID3D10Blob>();
	
	HRESULT hr = device->CreatePixelShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		NULL,
		m_pixelShader.GetAddressOf());
}


void DX11FragmentShader::Bind() {
	m_deviceContext->PSSetShader(m_pixelShader.Get(), 0, 0);
}

void DX11FragmentShader::UnBind() {
	//????????? figure out what is going on here
}