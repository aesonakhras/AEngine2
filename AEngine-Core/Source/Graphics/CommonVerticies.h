#pragma once
#include <DirectXMath.h>
#include "VertexAttribute.h"
#include "TextureFormat.h"
#include "IVertexDescription.h"

namespace AE::Graphics {
    struct StandardVertex {
        StandardVertex(
            DirectX::XMFLOAT3 _POS,
            DirectX::XMFLOAT3 _NORM,
            DirectX::XMFLOAT3 _TAN,
            DirectX::XMFLOAT3 _BITAN,
            DirectX::XMFLOAT2 _UV
        ) :
            POS(_POS), NORM(_NORM), TAN(_TAN), BITAN(_BITAN), UV(_UV) {}

        DirectX::XMFLOAT3 POS;
        DirectX::XMFLOAT3 NORM;
        DirectX::XMFLOAT3 TAN;
        DirectX::XMFLOAT3 BITAN;
        DirectX::XMFLOAT2 UV;
    };

    struct StandardVertexDescription : IVertexDescription
    {
        const static std::vector<VertexAttribute>& Get() {

            static std::vector<VertexAttribute> attributes {
                {"POSITION", 0, TextureFormat::RGB32F, 0},
                {"NORMAL", 0, TextureFormat::RGB32F, 12},
                {"TANGENT", 0, TextureFormat::RGB32F, 24},
                {"BITANGENT", 0, TextureFormat::RGB32F, 36},
                {"UV", 0, TextureFormat::RG32F, 48}
            };

            return attributes;
        }
    };
}