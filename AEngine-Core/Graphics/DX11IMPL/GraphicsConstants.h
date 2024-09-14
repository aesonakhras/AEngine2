#pragma once
#include "../Graphics/CommonVerticies.h"

namespace AE::Graphics {
    struct Constants {

        static const AE::Graphics::StandardVertex QuadVerticies[] =
        {
            {  DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)},
            { DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
            { DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
            { DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
        };

        static const unsigned int QuadIndicies[] = {
            0,1,2, 1,3,2,
        };

        static const AE::Graphics::StandardVertex CubeVerticies[] = {
                {DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)},
                {DirectX::XMFLOAT3(1.0f,  -1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)},
                {DirectX::XMFLOAT3(-1.0f,  1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)},
                {DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)},

                // Back Face
                {DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)},
                {DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),DirectX::XMFLOAT2(1.0f, 1.0f)},
                {DirectX::XMFLOAT3(-1.0f,  1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)},
                {DirectX::XMFLOAT3(1.0f,  1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)},
        };

        static const unsigned int CubeIndicies[] = {
            0,2,1, 2,3,1,
            1,3,5, 3,7,5,
            2,6,3, 3,6,7,
            4,5,7, 4,7,6,
            0,4,2, 2,4,6,
            0,1,4, 1,5,4
        };

        static const AE::Graphics::StandardVertex TriangleVerticies[] =
        {
            {DirectX::XMFLOAT3(0.0f, 0.73f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)},
            {DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)},
            {DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)}
        };

        static const unsigned int TriangleIndicies[] = { 0, 1, 2 };
    };
}