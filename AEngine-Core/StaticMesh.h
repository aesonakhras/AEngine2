#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "FragmentShader.h"

//This has grown to a large size, was expecting this to be a temp hack
struct StaticMesh {    
public:

    StaticMesh(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer,
        std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<FragmentShader> fragmentShader, Transform transform, ID3D11DeviceContext* deviceContext) :
        m_vertexBuffer(vertexBuffer),
        m_indexBuffer(indexBuffer),
        m_vertexShader(vertexShader),
        m_fragmentShader(fragmentShader),
        m_modelMatrix(DirectX::XMMatrixIdentity()),
        m_deviceContext(deviceContext)
    {
        SetTransform(transform);
    }

    //TODO: NONONONONONONONONONONONONONON
    //This is more of a Draw function
    void Draw(UINT stride, DirectX::XMMATRIX VPMatrix) {
        //update the mvp matrix in the vertexShader
        m_vertexShader->SetMVP(m_modelMatrix * VPMatrix);

        m_vertexBuffer->Bind(stride, 0);
        m_indexBuffer->Bind(DXGI_FORMAT_R32_UINT, 0);
        m_vertexShader->Bind();
        m_fragmentShader->Bind();

        m_deviceContext->DrawIndexed(m_indexBuffer->GetSize(), 0, 0);
    }

    void SetTransform(Transform transform) {
        //turn the transform into the model matrix

        m_modelMatrix = /*DirectX::XMMatrixIdentity() **/
            DirectX::XMMatrixScalingFromVector(transform.Scale) *
            DirectX::XMMatrixRotationRollPitchYawFromVector(transform.Rotation) *
            DirectX::XMMatrixTranslationFromVector(transform.Position);
            

       // m_modelMatrix = DirectX::XMMatrixIdentity();
            //DirectX::XMMatrixRotationRollPitchYawFromVector(transform.Rotation);
            //DirectX::XMMatrixTranslationFromVector(transform.Position);
            //DirectX::XMMatrixScalingFromVector(transform.Scale);
    }

private:
    std::shared_ptr<VertexBuffer> m_vertexBuffer = nullptr;
    std::shared_ptr<IndexBuffer> m_indexBuffer = nullptr;

    std::shared_ptr<VertexShader> m_vertexShader = nullptr;
    std::shared_ptr<FragmentShader> m_fragmentShader = nullptr;
    DirectX::XMMATRIX m_modelMatrix;
    //owns everything, this will lead to duplicate everything, but it will clea ip the core.cpp alot
    //also the core should not have any knowledge of what is going on
    ID3D11DeviceContext* m_deviceContext;
};