#include "DX11Sampler.h"
#include "Core/Debug.h"

using namespace AE::Graphics;

DX11Sampler::DX11Sampler(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr <ID3D11Device> device) : m_deviceContext(deviceContext) {
    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc {};


    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    sampDesc.MaxAnisotropy = 16;

    //TODO: Error checking
    HRESULT hr = device->CreateSamplerState(&sampDesc, m_pSampler.GetAddressOf());

    if (FAILED(hr)) {
        AE::Core::Debug::LogError("Unable to create sampler, invalid");
    }
}

void DX11Sampler::Bind(unsigned int slot) {
    m_deviceContext->PSSetSamplers(slot, 1, m_pSampler.GetAddressOf());
}

void DX11Sampler::Unbind(unsigned int slot) {
    //????
}
