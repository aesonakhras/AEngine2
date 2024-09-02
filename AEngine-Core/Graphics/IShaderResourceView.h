#pragma once

namespace AE::Core::Graphics {
    class IShaderResourceView
    {
    public:
        virtual ~IShaderResourceView() = default;

        virtual void Bind(unsigned int slot) = 0;
        virtual void Unbind(unsigned int slot) = 0;
    };
}