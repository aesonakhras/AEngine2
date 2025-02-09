#include "DX11ShaderResourceView.h"
#include "Graphics/DX11IMPL/DX11Utils.h"
#include "Graphics/DX11IMPL/DX11TextureResource.h"

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
        m_shaderResourceView(nullptr),
        m_use(info.use){

    if (info.use == TextureUse::DepthTexture) {
        CreateDepthStencilView(textureResource, DXGI_FORMAT_D32_FLOAT);
    }

    CreateShaderResourceView(textureResource, info);
}

void DX11ShaderResourceView::Bind(unsigned int slot) {
	m_deviceContext->PSSetShaderResources(slot, 1, m_shaderResourceView.GetAddressOf());
}

void DX11ShaderResourceView::Unbind(unsigned int slot) {
    ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
    m_deviceContext->PSSetShaderResources(slot, 1, nullSRV);
}

void DX11ShaderResourceView::CreateDepthStencilView(
    std::shared_ptr<DX11TextureResource> textureResource,
    DXGI_FORMAT format
) {
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> tempDsv;

    HRESULT hr = m_device->CreateDepthStencilView(
        static_cast<ID3D11Resource*>(textureResource->Get()),
        &dsvDesc,
        &tempDsv
    );

    if (FAILED(hr)) {
        AE::Core::Debug::LogError("Failed to create Depth Stencil View");
    }
    else {
        m_renderView = tempDsv;
    }
}

void DX11ShaderResourceView::CreateShaderResourceView(
    std::shared_ptr<DX11TextureResource> textureResource,
    const AE::Graphics::TextureCreateInfo& info
) {
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderViewDesc{};

    DXGI_FORMAT srvFormat = ConvertToDX11Format(textureResource->Format);
    if (srvFormat == DXGI_FORMAT_R32_TYPELESS) {
        srvFormat = DXGI_FORMAT_R32_FLOAT;
    }
    shaderViewDesc.Format = srvFormat;

    shaderViewDesc.ViewDimension = ConvertTODX11SRVDimension(textureResource->Type);

    UINT mipLevels = (info.generateMipMaps) ? -1 : info.mipLevels;

    if (info.type == TextureType::Cubemap) {
        shaderViewDesc.TextureCube.MipLevels = mipLevels;
        shaderViewDesc.TextureCube.MostDetailedMip = 0;
    }
    else {
        shaderViewDesc.Texture2D.MipLevels = mipLevels;
        shaderViewDesc.Texture2D.MostDetailedMip = 0;
    }

    auto hr = m_device->CreateShaderResourceView(
        static_cast<ID3D11Resource*>(textureResource->Get()),
        &shaderViewDesc,
        m_shaderResourceView.GetAddressOf()
    );

    if (FAILED(hr)) {
        AE::Core::Debug::LogError("Unable to create shader resource view");
    }

    //For now if we are not BC7 compression we need to create mip maps
    if (info.generateMipMaps) {
        AE::Core::Debug::Log("generating mips");
        m_deviceContext->GenerateMips(m_shaderResourceView.Get());
    }
}

void DX11ShaderResourceView::BindAsRenderTarget() {    
    switch (m_use) {
        case TextureUse::RenderTexture: {
                auto rtv = std::get<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>>(m_renderView);
                m_deviceContext->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
                break;
            }
        case TextureUse::DepthTexture: {
            auto dsv = std::get<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>>(m_renderView);
            m_deviceContext->OMSetRenderTargets(0, nullptr, dsv.Get());
            break;
        }
            default:
                AE::Core::Debug::LogError("Attempting to bind a non-renderable texture to OM Stage.");
                break;
    }
}

void DX11ShaderResourceView::UnBindAsRenderTarget() {
    switch (m_use) {
    case TextureUse::RenderTexture: {
        auto rtv = std::get<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>>(m_renderView);
        ID3D11RenderTargetView* nullRTV[1] = { nullptr };
        m_deviceContext->OMSetRenderTargets(1, nullRTV, nullptr);
        break;
    }
    case TextureUse::DepthTexture: {
        auto dsv = std::get<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>>(m_renderView);
        ID3D11DepthStencilView* nullDSV[1] = { nullptr };
        m_deviceContext->OMSetRenderTargets(0, nullptr, *nullDSV);
        break;
    }
    default:
        AE::Core::Debug::LogError("Attempting to bind a non-renderable texture to OM Stage.");
        break;
    }
}

void DX11ShaderResourceView::ClearRenderTarget() {
    switch (m_use) {
    case TextureUse::RenderTexture: {
        auto rtv = std::get<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>>(m_renderView);
        m_deviceContext->ClearRenderTargetView(rtv.Get(), RGBA{0.1f, 0.2f, 0.3f, 1.0f});
        break;
    }
    case TextureUse::DepthTexture: {
        auto dsv = std::get<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>>(m_renderView);
        m_deviceContext->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        break;
    }
    default:
        AE::Core::Debug::LogError("Attempting to bind a non-renderable texture to OM Stage.");
        break;
    }
}
