#include <DirectXMath.h>

#include "RenderSystem.h"

#include "Core/Components/Material.h"
#include "Core/Components/Transform.h"
#include "Core/Components/Mesh.h"
#include "Core/Components/Camera.h"

#include "Graphics/GraphicsManager.h"
#include "Graphics/CommonUBOs.h"

#include "Core/Scene/SceneManager.h"

using namespace AE::Core;
using namespace AE::Graphics;

void RenderSystem::Render() {
    //get scene ref
    SceneManager& sceneManager = SceneManager::GetInstance();
    GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

    graphicsManager.StartFrame();

    //before this grab the camera
    auto cameraView = sceneManager.Registry.view<Camera, Transform>();
    
    //create the vp
    auto cameraEntity = cameraView.front();

    auto& camera = cameraView.get<Camera>(cameraEntity);
    auto& cameraTransform = cameraView.get<Transform>(cameraEntity);

    auto projection = camera.ProjectionMatrix;
    auto view = cameraTransform.ToMatrix();

    auto vp2 = view * projection;

	auto staticMeshView = sceneManager.Registry.view<AE::Graphics::Material, Mesh, Transform>();

    DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    DirectX::XMVECTOR viewDir = DirectX::XMVector3Rotate(forward, cameraTransform.Rotation);

	for (auto entity : staticMeshView) {
		//get the model matrix and transform into the necessary thing

        auto& mesh = staticMeshView.get<Mesh>(entity);
        auto& material = staticMeshView.get<AE::Graphics::Material>(entity);
        auto& transform = staticMeshView.get<Transform>(entity);

        DirectX::XMMATRIX modelMatrix = transform.ToMatrix();

        auto mvp = modelMatrix * vp2;
        ////Thank you DirectX very nice
        ////TODO: this needs to go into the dx11 impl somewhere
        auto tMVP = DirectX::XMMatrixTranspose(mvp);

        mesh.VertexBuffer->Bind();
        mesh.IndexBuffer->Bind();

        //update ubo
        StandardUniformBuffer ubo{
            tMVP,
            modelMatrix,
            viewDir,
            {0,1,0,0}
        };

        //TODO: Material needs to be data only
        material.SetUBO(&ubo, sizeof(StandardUniformBuffer));
        material.Bind();

        graphicsManager.Draw(mesh.IndexBuffer->Count);

        //graphicsManager;
	}

    graphicsManager.PresentFrame();
}