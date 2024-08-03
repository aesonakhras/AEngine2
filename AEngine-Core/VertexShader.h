#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Transform.h"
#include "VertexLayout.h"

#include <wrl/client.h>

#include "../DX11_InputLayout.h"
#include "../AEngineVertexTypes.h"
#include "../DX11_Buffer.h"
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
	Microsoft::WRL::ComPtr <ID3D11VertexShader> m_pVS = nullptr;
	Microsoft::WRL::ComPtr <ID3D10Blob> m_VS = nullptr;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> m_deviceContext = nullptr;
	DirectX::XMFLOAT4 m_color;

	std::shared_ptr <DX11_InputLayout<AEngineVertexTypes::VERTEX2>> m_layout = nullptr;

	std::shared_ptr<DX11_Buffer> m_constantBuffer = nullptr;
};