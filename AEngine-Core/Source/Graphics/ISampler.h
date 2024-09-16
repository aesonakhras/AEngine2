#pragma once

namespace AE::Graphics {

    class ISampler
    {
        public:
            virtual ~ISampler() = default;

            virtual void Bind(unsigned int slot) = 0;
            virtual void Unbind(unsigned int slot) = 0;
    };
}