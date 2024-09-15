#pragma once
#include "Graphics/IBuffer.h"
#include "../Transform.h"
#include "../Graphics/Material.h"
#include "../Core/WorldObject.h"


//This has grown to a large size, was expecting this to be a temp hack
struct StaticMesh {    
public:
    StaticMesh(std::shared_ptr<AE::Graphics::IBuffer> vertexBuffer,
        std::shared_ptr<AE::Graphics::IBuffer> indexBuffer,
        std::shared_ptr<AE::Graphics::Material> material,
        std::shared_ptr <AE::Core::WorldObject> worldObject) :
        m_vertexBuffer(vertexBuffer),
        m_indexBuffer(indexBuffer),
        m_material(material),
        m_worldObject(worldObject) { }

    void Bind(DirectX::XMMATRIX vp) const {
        m_vertexBuffer->Bind();
        m_indexBuffer->Bind();

        DirectX::XMMATRIX modelMatrix = m_worldObject->GetMatrix();

        auto mvp = modelMatrix * vp;
        //Thank you DirectX very nice
        //TODO: AHHHHHHH this needs to go into the dx11 impl somewhere
        auto tMVP = DirectX::XMMatrixTranspose(mvp);

        m_material->SetUBO(&tMVP, sizeof(DirectX::XMMATRIX));
        m_material->Bind();
    }

    int GetCount() const {
        return m_indexBuffer->Count;
    }

    //TODO: Hide this later on, for now I want the thing rendering again
    std::shared_ptr<AE::Core::WorldObject> m_worldObject = nullptr;

    private:
    std::shared_ptr<AE::Graphics::IBuffer> m_vertexBuffer = nullptr;
    std::shared_ptr<AE::Graphics::IBuffer> m_indexBuffer = nullptr;

    std::shared_ptr<AE::Graphics::Material> m_material = nullptr;
};