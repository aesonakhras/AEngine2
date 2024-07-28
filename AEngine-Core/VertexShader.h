#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Transform.h"
#include "VertexLayout.h"

#include <wrl/client.h>

#include "../DX11_InputLayout.h"
#include "../AEngineVertexTypes.h"
#include <memory>

class VertexShader {
public:
	VertexShader(Microsoft::WRL::ComPtr <ID3D11DeviceContext> deviceContext,
		Microsoft::WRL::ComPtr <ID3D11Device> device,
		LPCWSTR pFileName, DirectX::XMFLOAT4 color, VextexLayout layout);
	void Bind();
	
	//TODO: This cannot be on the VertexShader
	void SetMVP(DirectX::XMMATRIX mvp);
	~VertexShader();
private:
	void setupConstantbuffer(Microsoft::WRL::ComPtr <ID3D11Device> device, DirectX::XMFLOAT4 color);

	Microsoft::WRL::ComPtr <ID3D11VertexShader> m_pVS = nullptr;
	Microsoft::WRL::ComPtr <ID3D10Blob> m_VS = nullptr;

	Microsoft::WRL::ComPtr <ID3D11DeviceContext> m_deviceContext = nullptr;
	ID3D11Buffer* m_pConstantBuffer = nullptr;
	DirectX::XMFLOAT4 m_color;

	//Todo: convert to smart pointer or something
	std::shared_ptr <DX11_InputLayout<AEngineVertexTypes::VERTEX2>> m_layout = nullptr;
};