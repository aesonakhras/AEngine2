#include <DirectXMath.h>

#include "RenderSystem.h"

#include "Core/Components/Material.h"
#include "Core/Components/Transform.h"
#include "Graphics/Mesh.h"
#include "Core/Components/Camera.h"
#include "Core/Components/SkyBox.h"

#include "Graphics/GraphicsManager.h"

#include "Core/Scene/SceneManager.h"

using namespace AE::Core;
using namespace AE::Graphics;


//TODO: Break this into multiple functions
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
    auto view = DirectX::XMMatrixInverse(nullptr, cameraTransform.WorldMatrix);

    

    auto vp2 = view * projection;

	auto staticMeshView = sceneManager.Registry.view<AE::Graphics::Material, AE::Graphics::Mesh, Transform>();

    DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    DirectX::XMVECTOR viewDir = DirectX::XMVector3Rotate(forward, cameraTransform.GetLocalRotation());

	for (auto entity : staticMeshView) {
		//get the model matrix and transform into the necessary thing

        auto& mesh = staticMeshView.get<AE::Graphics::Mesh>(entity);
        auto& material = staticMeshView.get<AE::Graphics::Material>(entity);
        auto& transform = staticMeshView.get<Transform>(entity);

        DirectX::XMMATRIX modelMatrix = transform.WorldMatrix;

        auto mvp = modelMatrix * vp2;
        ////Thank you DirectX very nice
        ////TODO: this needs to go into the dx11 impl somewhere
        auto tMVP = DirectX::XMMatrixTranspose(mvp);
        //auto tModelMatrix = DirectX::XMMatrixTranspose(modelMatrix);

        mesh.VertexBuffer->Bind(0);
        mesh.IndexBuffer->Bind(0);

        DirectX::XMVECTOR dirLight = { 0,1,0,0 };

        material.SetUniform("MVP", mvp);
        material.SetUniform("Model", modelMatrix);
        material.SetUniform("ViewDir", viewDir);
        material.SetUniform("DirLight", dirLight);

        material.Bind();

        graphicsManager.Draw(mesh.IndexBuffer->Count);

        //graphicsManager;
	}
    
    MaybeRenderSkyBox(view, projection);

    graphicsManager.PresentFrame();
}

void RenderSystem::MaybeRenderSkyBox(DirectX::XMMATRIX view, DirectX::XMMATRIX projection) {

    SceneManager& sceneManager = SceneManager::GetInstance();
    GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

    graphicsManager.SetDepthState(true);
    //now do the skybox
    auto skyBoxView = sceneManager.Registry.view<SkyBox, Mesh, Material>();
    if (skyBoxView.begin() != skyBoxView.end()) {

        auto skyboxEntity = skyBoxView.front();

        auto& skyBoxMesh = skyBoxView.get<Mesh>(skyboxEntity);
        auto& skyBoxMaterial = skyBoxView.get<AE::Graphics::Material>(skyboxEntity);

        //calculate the view matrix with no translation
        DirectX::XMMATRIX finalViewMatrix = view;

        finalViewMatrix.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, finalViewMatrix.r[3].m128_f32[3]);

        finalViewMatrix = finalViewMatrix * projection;

        auto tfinalViewMatrix = DirectX::XMMatrixTranspose(finalViewMatrix);

        skyBoxMaterial.SetUniform("vpNoPosition", tfinalViewMatrix);
        skyBoxMaterial.Bind();
        skyBoxMesh.VertexBuffer->Bind(0);
        skyBoxMesh.IndexBuffer->Bind(0);

        graphicsManager.Draw(skyBoxMesh.IndexBuffer->Count);
    }

    graphicsManager.SetDepthState(false);
}