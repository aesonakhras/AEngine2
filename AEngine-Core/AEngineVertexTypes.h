#pragma once

#include <DirectXMath.h>
#include <../VertexLayout.h>

//This is mostly for now, a quick hack to remove some text from the core.cpp 
//until I know where to put these
namespace AEngineVertexTypes {
    struct VERTEX {
        VERTEX(DirectX::XMFLOAT3 _POS, DirectX::XMFLOAT2 _UV) :
            POS(_POS), UV(_UV) {}

        DirectX::XMFLOAT3 POS;
        DirectX::XMFLOAT2 UV;
    };

    struct VERTEX2 {
        VERTEX2(DirectX::XMFLOAT3 _POS, DirectX::XMFLOAT2 _UV) :
            POS(_POS), UV(_UV) {}

        DirectX::XMFLOAT3 POS;
        DirectX::XMFLOAT2 UV;

        static VextexLayout GetLayout() {
            return { 
                {
                    {"POSITION", 0, AE_R32B32G32, 0},
                    {"UV", 0, AE_R32B32G32, 12},
                },
                sizeof(VERTEX2)
            };
        }
    };
}