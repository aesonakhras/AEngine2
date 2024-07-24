#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Transform.h"
#include "VertexLayout.h"

class VertexShader {
public:
	VertexShader(ID3D11DeviceContext* deviceContext, ID3D11Device* device, LPCWSTR pFileName, DirectX::XMFLOAT4 color, VextexLayout layout);
	void Bind();
	
	//TODO: This cannot be on the VertexShader
	void SetMVP(DirectX::XMMATRIX mvp);
	~VertexShader();
private:
	void setupConstantbuffer(ID3D11Device* device, DirectX::XMFLOAT4 color);

	ID3D11VertexShader* m_pVS = nullptr;
	ID3D10Blob* m_VS = nullptr;

	ID3D11InputLayout* m_layout = nullptr;
	D3D11_INPUT_ELEMENT_DESC m_ied[2] =

	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	ID3D11DeviceContext* m_deviceContext = nullptr;
	ID3D11Buffer* m_pConstantBuffer = nullptr;
	DirectX::XMFLOAT4 m_color;
};