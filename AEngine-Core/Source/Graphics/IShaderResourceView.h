#pragma once

namespace AE::Graphics {
    class IShaderResourceView
    {
    public:
        virtual ~IShaderResourceView() = default;

        virtual void Bind(unsigned int slot) = 0;
        virtual void Unbind(unsigned int slot) = 0;

        virtual void BindAsRenderTarget() = 0;
        virtual void UnBindAsRenderTarget() = 0;

        virtual void ClearRenderTarget() = 0;
    };
}