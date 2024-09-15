#include "DX11VertexLayout.h"
#include "../Graphics/VertexAttribute.h"
#include "../DX11IMPL/DX11Utils.h"
#include "../Graphics/IVertexDescription.h"
#include "../Core/Debug.h"

using namespace AE::Graphics;

DX11VertexLayout::DX11VertexLayout(
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	Microsoft::WRL::ComPtr<ID3D11Device> device,
	Microsoft::WRL::ComPtr<ID3D10Blob> shaderBlob
) : m_deviceContext(deviceContext),
	m_device(device),
	m_shaderBlob(shaderBlob) {};


void DX11VertexLayout::Build(const std::vector<VertexAttribute>& vertexAttributes) {
	if ((vertexAttributes.size() == 0)) {
		AE::Core::Debug::LogError("Cannot create vertexLayout with no attributes");
		return;
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> elementDescs{ };

	for (const VertexAttribute& layoutElement : vertexAttributes) {
		D3D11_INPUT_ELEMENT_DESC desc;
		desc.SemanticName = layoutElement.name.c_str();
		desc.SemanticIndex = layoutElement.SemanticIndex;
		desc.Format = AE::Graphics::ConvertToDX11Format(layoutElement.format);
		desc.InputSlot = 0;
		desc.AlignedByteOffset = layoutElement.offset;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;

		elementDescs.push_back(desc);
	}

	//set up the layout
	m_device->CreateInputLayout(
		elementDescs.data(),
		elementDescs.size(),
		m_shaderBlob->GetBufferPointer(),
		m_shaderBlob->GetBufferSize(),
		m_layout.GetAddressOf());
};

void DX11VertexLayout::Bind() {
	m_deviceContext->IASetInputLayout(m_layout.Get());
};


void DX11VertexLayout::UnBind() {

};