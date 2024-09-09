#pragma once
#include "IBuffer.h"
#include "VertexShader.h"
#include "../Graphics/IFragmentShader.h"

//This has grown to a large size, was expecting this to be a temp hack
struct StaticMesh {    
public:

    StaticMesh(std::shared_ptr<IBuffer> vertexBuffer, std::shared_ptr<IBuffer> indexBuffer,
        std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<IFragmentShader> fragmentShader, Transform transform) :
        m_vertexBuffer(vertexBuffer),
        m_indexBuffer(indexBuffer),
        m_vertexShader(vertexShader),
        m_fragmentShader(fragmentShader),
        m_modelMatrix(DirectX::XMMatrixIdentity())
    {
        SetTransform(transform);
    }

    void SetTransform(Transform transform) {
        //turn the transform into the model matrix

        m_modelMatrix =
            DirectX::XMMatrixScalingFromVector(transform.Scale) *
            DirectX::XMMatrixRotationRollPitchYawFromVector(transform.Rotation) *
            DirectX::XMMatrixTranslationFromVector(transform.Position);
    }

    //TODO: This will be cleaned up later
    std::shared_ptr<IBuffer> m_vertexBuffer = nullptr;
    std::shared_ptr<IBuffer> m_indexBuffer = nullptr;

    std::shared_ptr<VertexShader> m_vertexShader = nullptr;
    std::shared_ptr<IFragmentShader> m_fragmentShader = nullptr;
    DirectX::XMMATRIX m_modelMatrix;
};