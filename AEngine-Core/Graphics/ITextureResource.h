#pragma once

namespace AE::Core::Graphics {
    class ITextureResource {
        public:
            virtual ~ITextureResource() = default;
        
            virtual void* Get() const = 0;
    };
}