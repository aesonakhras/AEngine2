#pragma once
#include <d3d11.h>
#include <wrl/client.h>

#include "Graphics/ISampler.h"

namespace AE::Graphics {
    class DX11Sampler : public ISampler
    {
        public:
            DX11Sampler(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, Microsoft::WRL::ComPtr <ID3D11Device> device, bool isDepth);

            virtual void Bind(unsigned int slot) override;
            virtual void Unbind(unsigned int slot) override;

        private:
            Microsoft::WRL::ComPtr <ID3D11DeviceContext> m_deviceContext;
            Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;
    };
}