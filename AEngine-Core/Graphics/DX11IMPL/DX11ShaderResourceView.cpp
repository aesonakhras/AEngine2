#include "DX11ShaderResourceView.h"
#include "../Core/Debug.h"

using namespace AE::Graphics;

DX11ShaderResourceView::DX11ShaderResourceView(Microsoft::WRL::ComPtr<ID3D11DeviceContext>deviceContext, Microsoft::WRL::ComPtr <ID3D11Device> device, ID3D11Resource* texture, D3D11_SRV_DIMENSION viewDimension) :
    m_deviceContext(deviceContext),
        m_device(device),
        m_shaderResourceView(nullptr) {

        //TODO: Update this to be better during the refactor
        D3D11_SHADER_RESOURCE_VIEW_DESC shaderViewDesc;
        ZeroMemory(&shaderViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        shaderViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        shaderViewDesc.ViewDimension = viewDimension;


        auto hr = device->CreateShaderResourceView(texture, NULL, m_shaderResourceView.GetAddressOf());

        if (FAILED(hr)) {
            AE::Core::Debug::LogError("Unable to create shader resource view");
        }
}

void DX11ShaderResourceView::Bind(unsigned int slot) {
	m_deviceContext->PSSetShaderResources(slot, 1, m_shaderResourceView.GetAddressOf());
}