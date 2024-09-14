#pragma once
#include <DirectXMath.h>
#include "../Graphics/VertexAttribute.h"
#include "../Graphics/TextureFormat.h"
#include "../Graphics/IVertexDescription.h"

namespace AE::Graphics {
    struct StandardVertex {
        StandardVertex(DirectX::XMFLOAT3 _POS, DirectX::XMFLOAT2 _UV) :
            POS(_POS), UV(_UV) {}

        DirectX::XMFLOAT3 POS;
        DirectX::XMFLOAT2 UV;
    };

    struct StandardVertexDescription : IVertexDescription
    {
        const static std::vector<VertexAttribute>& Get() {

            static std::vector<VertexAttribute> attributes {
                {"POSITION", 0, TextureFormat::RGB32F, 0},
                {"UV", 0, TextureFormat::RG32F, 12}
            };

            return attributes;
        }
    };
}