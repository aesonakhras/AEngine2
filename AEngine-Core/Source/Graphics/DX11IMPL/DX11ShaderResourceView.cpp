#include "DX11ShaderResourceView.h"
#include "Graphics/DX11IMPL/DX11Utils.h"
#include "Graphics/DX11IMPL/DX11TextureResource.h"
#include "Graphics/TextureCreateInfo.h"
#include "Core/Debug.h"

using namespace AE::Graphics;

DX11ShaderResourceView::DX11ShaderResourceView(
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>deviceContext,
    Microsoft::WRL::ComPtr <ID3D11Device> device,
    std::shared_ptr<DX11TextureResource> textureResource,
    const AE::Graphics::TextureCreateInfo& info
) :
    m_deviceContext(deviceContext),
        m_device(device),
        m_shaderResourceView(nullptr) {

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderViewDesc {};
        shaderViewDesc.Format = ConvertToDX11Format(textureResource->Format);

        shaderViewDesc.ViewDimension = ConvertTODX11SRVDimension(textureResource->Type);

        if ((info.mipLevels <= 1) && (textureResource->Format != TextureFormat::BC7_UNORM)) {
            shaderViewDesc.Texture2D.MipLevels = -1;
            shaderViewDesc.Texture2D.MostDetailedMip = 0;
        }

        auto hr = device->CreateShaderResourceView(
            static_cast<ID3D11Resource*>(textureResource->Get()),
            NULL,
            m_shaderResourceView.GetAddressOf()
        );

        if (FAILED(hr)) {
            AE::Core::Debug::LogError("Unable to create shader resource view");
        }


        //For now if we are not BC7 compression we need to create mip maps
        if ((info.mipLevels <= 1) && (textureResource->Format != TextureFormat::BC7_UNORM)) {
            AE::Core::Debug::Log("generating mips");
            deviceContext->GenerateMips(m_shaderResourceView.Get());
        }
}

void DX11ShaderResourceView::Bind(unsigned int slot) {
	m_deviceContext->PSSetShaderResources(slot, 1, m_shaderResourceView.GetAddressOf());
}