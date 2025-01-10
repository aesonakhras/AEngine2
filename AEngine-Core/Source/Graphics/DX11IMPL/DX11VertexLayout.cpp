#include "Graphics/VertexAttribute.h"
#include "Graphics/IVertexDescription.h"

#include "Core/Debug.h"

#include "DX11Utils.h"
#include "DX11VertexLayout.h"

using namespace AE::Graphics;

DX11VertexLayout::DX11VertexLayout(
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	Microsoft::WRL::ComPtr<ID3D11Device> device
) : m_deviceContext(deviceContext),
	m_device(device) {};


void DX11VertexLayout::Build(
	const std::vector<VertexAttribute>& vertexAttributes,
	Microsoft::WRL::ComPtr<ID3D10Blob> shaderBlob
) {
	if ((vertexAttributes.size() == 0)) {
		AE::Core::Debug::LogError("Cannot create vertexLayout with no attributes");
		return;
	}

	for (const VertexAttribute& layoutElement : vertexAttributes) {
		D3D11_INPUT_ELEMENT_DESC desc;
		desc.SemanticName = layoutElement.name.c_str();
		desc.SemanticIndex = layoutElement.SemanticIndex;
		desc.Format = AE::Graphics::ConvertToDX11Format(layoutElement.format);
		desc.InputSlot = 0;
		desc.AlignedByteOffset = layoutElement.offset;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;

		m_elementDescs.push_back(desc);
	}

	Rebuild(shaderBlob);
};

void DX11VertexLayout::Rebuild(Microsoft::WRL::ComPtr<ID3D10Blob> shaderBlob) {
	//set up the layout
	HRESULT hr = m_device->CreateInputLayout(
		m_elementDescs.data(),
		m_elementDescs.size(),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		m_layout.GetAddressOf());

	if (FAILED(hr)) {
		AE::Core::Debug::LogError("Failed to create layout");
	}
}

void DX11VertexLayout::Bind() {
	m_deviceContext->IASetInputLayout(m_layout.Get());
};


void DX11VertexLayout::UnBind() {

};