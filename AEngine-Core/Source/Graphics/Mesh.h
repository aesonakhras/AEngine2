#pragma once
#include <memory>
#include <DirectXMath.h>

#include "Graphics/IBuffer.h"

#include "Physics/Bounds.h"

//TODO: This should be a component and not a graphics part
namespace AE::Graphics {
    struct Mesh {
    public:
        Mesh(std::shared_ptr<AE::Graphics::IBuffer> vertexBuffer,
            std::shared_ptr<AE::Graphics::IBuffer> indexBuffer,
            AE::Physics::AABB aabb) :
            VertexBuffer(vertexBuffer),
            IndexBuffer(indexBuffer),
            Aabb(aabb) { }

        //~Mesh() {
        //    int i = 1;

        //    int y = i / 2;


        //}

        std::shared_ptr<AE::Graphics::IBuffer> VertexBuffer = nullptr;
        std::shared_ptr<AE::Graphics::IBuffer> IndexBuffer = nullptr;
        AE::Physics::AABB Aabb;
    };
}