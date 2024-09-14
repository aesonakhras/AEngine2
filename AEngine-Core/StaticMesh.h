#pragma once
#include "IBuffer.h"
#include "../Transform.h"
#include "../Graphics/IVertexShader.h"
#include "../Graphics/IFragmentShader.h"

//This has grown to a large size, was expecting this to be a temp hack
struct StaticMesh {    
public:

    StaticMesh(std::shared_ptr<AE::Graphics::IBuffer> vertexBuffer, std::shared_ptr<AE::Graphics::IBuffer> indexBuffer,
        std::shared_ptr<AE::Graphics::IVertexShader> vertexShader, std::shared_ptr<AE::Graphics::IFragmentShader> fragmentShader, Transform transform) :
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

    void Bind() const {
        m_vertexBuffer->Bind();
        m_indexBuffer->Bind();

        m_vertexShader->Bind();
        m_fragmentShader->Bind();
    }

    int GetCount() const {
        return m_indexBuffer->Count;
    }

    private:
    std::shared_ptr<AE::Graphics::IBuffer> m_vertexBuffer = nullptr;
    std::shared_ptr<AE::Graphics::IBuffer> m_indexBuffer = nullptr;

    std::shared_ptr<AE::Graphics::IVertexShader> m_vertexShader = nullptr;
    std::shared_ptr<AE::Graphics::IFragmentShader> m_fragmentShader = nullptr;
    DirectX::XMMATRIX m_modelMatrix;
};