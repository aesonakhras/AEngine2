#pragma once

#include <vector>

#include <../IInputLayout.h>
#include <../VertexLayout.h>
#include <wrl/client.h>
#include <d3d11.h>

template <typename T>
class DX11_InputLayout : public IInputLayout<T> {
private:
	Microsoft::WRL::ComPtr <ID3D11InputLayout> m_layout = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext = nullptr;

	DXGI_FORMAT ConvertToDX11Format(AE_FORMAT format) {
		switch (format)
		{
		case AE_R32B32G32:
			return DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case AE_R8B8G8A8:
			return DXGI_FORMAT_R8G8B8A8_UINT;
			break;
		default:
			//TODO: Some sort of error here
			return DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		}
	}

public:
	DX11_InputLayout(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, 
		Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr <ID3DBlob> vertexShader) : m_deviceContext(deviceContext) {
		VextexLayout layout = T::GetLayout();

		std::vector<D3D11_INPUT_ELEMENT_DESC> elementDescs{ };

		for (const VertexAttribute& layoutElement : layout.Attributes) {
			D3D11_INPUT_ELEMENT_DESC desc;
			desc.SemanticName = layoutElement.name.c_str();
			desc.SemanticIndex = layoutElement.SemanticIndex;
			desc.Format = ConvertToDX11Format(layoutElement.format);
			desc.InputSlot = 0;
			desc.AlignedByteOffset = layoutElement.offset;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;

			elementDescs.push_back(desc);
		}

		//set up the layout
		device->CreateInputLayout(elementDescs.data(), elementDescs.size(), vertexShader->GetBufferPointer(),
			vertexShader->GetBufferSize(), &m_layout);
	}

	void Bind() override {
		m_deviceContext->IASetInputLayout(m_layout.Get());
	}

	void UnBind() override {
		//idk
	}
};