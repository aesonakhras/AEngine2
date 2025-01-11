#include "DX11ShaderResourceView.h"
#include "Graphics/DX11IMPL/DX11Utils.h"
#include "Graphics/DX11IMPL/DX11TextureResource.h"
#include "Core/Debug.h"

using namespace AE::Graphics;

DX11ShaderResourceView::DX11ShaderResourceView(
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>deviceContext,
    Microsoft::WRL::ComPtr <ID3D11Device> device,
    std::shared_ptr<DX11TextureResource> textureResource
) :
    m_deviceContext(deviceContext),
        m_device(device),
        m_shaderResourceView(nullptr) {

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderViewDesc {};
        shaderViewDesc.Format = ConvertToDX11Format(textureResource->Format);

        shaderViewDesc.ViewDimension = ConvertTODX11SRVDimension(textureResource->Type);

        auto hr = device->CreateShaderResourceView(
            static_cast<ID3D11Resource *>(textureResource->Get()),
            NULL,
            m_shaderResourceView.GetAddressOf()
        );

        if (FAILED(hr)) {
            AE::Core::Debug::LogError("Unable to create shader resource view");
        }
}

void DX11ShaderResourceView::Bind(unsigned int slot) {
	m_deviceContext->PSSetShaderResources(slot, 1, m_shaderResourceView.GetAddressOf());
}