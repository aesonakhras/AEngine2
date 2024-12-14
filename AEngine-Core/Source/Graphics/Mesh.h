#pragma once
#include <memory>
#include <DirectXMath.h>

#include "Graphics/IBuffer.h"

namespace AE::Graphics {
    struct Mesh {
    public:
        Mesh(std::shared_ptr<AE::Graphics::IBuffer> vertexBuffer,
            std::shared_ptr<AE::Graphics::IBuffer> indexBuffer) :
            VertexBuffer(vertexBuffer),
            IndexBuffer(indexBuffer) { }

        std::shared_ptr<AE::Graphics::IBuffer> VertexBuffer = nullptr;
        std::shared_ptr<AE::Graphics::IBuffer> IndexBuffer = nullptr;
    };
}