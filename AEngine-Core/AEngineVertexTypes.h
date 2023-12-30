#pragma once

#include <DirectXMath.h>
//This is mostly for now, a quick hack to remove some text from the core.cpp 
//until I know where to put these
namespace AEngineVertexTypes {
    struct VERTEX {
        VERTEX(DirectX::XMFLOAT3 _POS, DirectX::XMFLOAT2 _UV) :
            POS(_POS), UV(_UV) {}

        DirectX::XMFLOAT3 POS;
        DirectX::XMFLOAT2 UV;
    };
}