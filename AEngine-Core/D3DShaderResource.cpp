#include "D3DShaderResource.h"
#include <iostream>
D3DShaderResource::D3DShaderResource(ID3D11DeviceContext* deviceContext, ID3D11Device* device, ID3D11Texture2D* texture, D3D11_SRV_DIMENSION viewDimension) :
    m_deviceContext(deviceContext),
    m_device(device),
    m_shaderResourceView(nullptr) {

    //TODO: Update this to be better during the refactor
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderViewDesc;
    ZeroMemory(&shaderViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    shaderViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    shaderViewDesc.ViewDimension = viewDimension;
    

    auto hr = device->CreateShaderResourceView(texture, NULL, &m_shaderResourceView);

    if (FAILED(hr)) {
        std::cout << "Shader Resource Failed" << std::endl;
    }
}

void D3DShaderResource::Bind() {
    //TODO: Determine how to adjust the bind point
    m_deviceContext->PSSetShaderResources(0, 1, &m_shaderResourceView);
}