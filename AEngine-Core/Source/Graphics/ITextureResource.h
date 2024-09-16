#pragma once

namespace AE::Graphics {
    class ITextureResource {
        public:
            virtual ~ITextureResource() = default;
        
            virtual void* Get() const = 0;
    };
}