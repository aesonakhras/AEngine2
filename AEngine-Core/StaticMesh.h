#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "FragmentShader.h"

//This has grown to a large size, was expecting this to be a temp hack
struct StaticMesh {    
public:

    StaticMesh(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer,
        std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<FragmentShader> fragmentShader, Transform transform) :
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
    std::shared_ptr<VertexBuffer> m_vertexBuffer = nullptr;
    std::shared_ptr<IndexBuffer> m_indexBuffer = nullptr;

    std::shared_ptr<VertexShader> m_vertexShader = nullptr;
    std::shared_ptr<FragmentShader> m_fragmentShader = nullptr;
    DirectX::XMMATRIX m_modelMatrix;
};