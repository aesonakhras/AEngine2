#include "Sampler.h"

Sampler::Sampler(ID3D11DeviceContext* deviceContext, ID3D11Device* device) : m_deviceContext(deviceContext){
    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    //TODO: Error checking
    device->CreateSamplerState(&sampDesc, &m_pSampler);
}

void Sampler::Bind() {
    m_deviceContext->PSSetSamplers(0, 1, &m_pSampler);
}

Sampler::~Sampler() {

}
