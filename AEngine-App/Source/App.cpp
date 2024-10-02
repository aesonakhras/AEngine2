#include <memory>
#include <vector>

#include <entt/entt.hpp>

#include "Core/Core.h"

//TODO: determine if this should just be in a common.h or core.h or something
#include "Graphics/GraphicsManager.h"

#include "Core/Components/Material.h"
#include "Graphics/CommonVerticies.h"
#include "Graphics/CommonUBOs.h"

#include "Core/Scene/SceneManager.h"

#include "FileManagment/MeshData.h"
#include "FileManagment/FileImporter.h"

#include "System/Audio/AudioManager.h"
#include "System/Input/InputManager.h"

#include "Core/Factories/StaticMeshFactory.h"
#include "Core/Factories/CameraFactory.h"

#include "Systems/PlayerSystem.h"
#include "Factories/PlayerFactory.h"

#include <iostream>

using namespace AE::Core;
using namespace AE::Graphics;
using namespace AE::App;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

//Camera variables
float fov = 1.0472f;
float aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
float nearZ = 0.01f;
float farZ = 1000.0f;
DirectX::XMVECTOR Lookat = { 0.0f, 0.0f, -1.0f, 0.0f };

//set up necessary stuff for now, will create resource managers soon
std::shared_ptr<IBuffer> CatVB = nullptr;
std::shared_ptr <IBuffer> CatIB = nullptr;

std::shared_ptr <Material> CatMat1 = nullptr;
std::shared_ptr <Material> CatMat2 = nullptr;

std::shared_ptr <Texture> CatTexture1 = nullptr;
std::shared_ptr <Texture> CatTexture2 = nullptr;

std::shared_ptr<AE::Graphics::ISampler> CatSampler = nullptr;

entt::entity camera;

AE::App::PlayerSystem playerSystem;

void textureSetup() {
    GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

    AE::Graphics::TextureCreateInfo textureData = FileImporter::ImportTexture(std::string("Assets/Rock/basecolor.png"));

    textureData.depth = 1;
    textureData.mipLevels = 1;
    textureData.bindFlags = AE::Graphics::ShaderResource;
    textureData.generateMipMaps = false;
    textureData.arraySize = 1;
    textureData.sampleCount = 1;

    CatTexture1 = graphicsManager.CreateTexture(textureData);

    AE::Graphics::TextureCreateInfo textureData2 = FileImporter::ImportTexture(std::string("Assets/Rock/normal.png"));

    textureData2.depth = 1;
    textureData2.mipLevels = 1;
    textureData2.bindFlags = AE::Graphics::ShaderResource;
    textureData2.generateMipMaps = false;
    textureData2.arraySize = 1;
    textureData2.sampleCount = 1;

    CatTexture2 = graphicsManager.CreateTexture(textureData2);

    CatSampler = graphicsManager.CreateSampler();
}

void loadCommonResoureces() {
    GraphicsManager& graphicsManager = GraphicsManager::GetInstance();
    MeshData meshData = FileImporter::ImportMesh(std::string("Assets/Rock/rock.obj"));

    //buffers
    CatVB = graphicsManager.CreateBuffer(
        (void*)meshData.vertexData,
        meshData.vertexCount,
        sizeof(AE::Graphics::StandardVertex),
        AE::Graphics::BufferType::Vertex
    );

    CatIB = graphicsManager.CreateBuffer(
        (void*)meshData.indexData,
        meshData.indexCount,
        sizeof(unsigned int),
        AE::Graphics::BufferType::Index
    );

    StandardUniformBuffer mvp;
    mvp.mWorldViewProj = DirectX::XMMatrixIdentity();

    CatMat1 = graphicsManager.CreateMaterial(
        "Assets/shaders.shader",
        AE::Graphics::StandardVertexDescription::Get(),
        &mvp,
        sizeof(StandardUniformBuffer)
    );

    CatMat2 = graphicsManager.CreateMaterial(
        "Assets/shaders.shader",
        AE::Graphics::StandardVertexDescription::Get(),
        &mvp,
        sizeof(StandardUniformBuffer)
    );

    //textures
    textureSetup();

    CatMat1->SetTexture("diffuse1", 0, CatTexture1, CatSampler);
    CatMat1->SetTexture("diffuse2", 1, CatTexture2, CatSampler);

    CatMat2->SetTexture("diffuse1", 0, CatTexture1, CatSampler);
    CatMat2->SetTexture("diffuse2", 1, CatTexture2, CatSampler);
}

void SetupScene() {
    //get scene ref
    AE::Core::SceneManager& sceneManager = AE::Core::SceneManager::GetInstance();

    camera = AE::Core::CameraFactory::Create(
        sceneManager.Registry,
        fov,
        aspectRatio,
        nearZ,
        farZ,
        {0.0, 0.0, 0.0},
        Lookat
    );

    loadCommonResoureces();

    //add the two kitty cats
    Transform cat1Start = {
        { 0 , 0.0f, 1 , 1.0f },
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 1.5f, 0.0f, 0.0f, 0.0f }),
        {1.0f, 1.0f, 1.0f} 
    };

    Transform cat2Start = {
        { 0 , 0.0f, 15.0f, 1.0f },
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 1.5f, 0.0f, 0.0f, 0.0f }),
        {1.0f, 1.0f, 1.0f}
    };

    Mesh catMesh = { CatVB, CatIB };

    //auto mesh1 = StaticMeshFactory::CreateStaticMesh(sceneManager.Registry, catMesh, *CatMat1.get(), cat1Start);

    auto player = PlayerFactory::Create(sceneManager.Registry, catMesh, *CatMat2.get(), cat2Start);
    //auto mesh2 = StaticMeshFactory::CreateStaticMesh(sceneManager.Registry, catMesh, *CatMat2.get(), cat2Start);
}

void OnPressed() {
    AE::System::AudioManager::GetInstance().PlayAudio("gunshot");
}

void Update(float32 deltaTime,
    AE::Core::JobSystem& jobSystem,
    AE::Core::CommandBuffer& commandBuffer
) {
    AE::Core::SceneManager& sceneManager = AE::Core::SceneManager::GetInstance();

    playerSystem.Update(deltaTime, sceneManager.Registry, jobSystem, commandBuffer);
}

//startup
void StartApp() {
    SetupScene();

    AE::System::InputManager::GetInstance().RegisterButtonEvent(AE::System::Button::W, AE::System::InputState::Pressed, OnPressed);

    AE::System::AudioManager::GetInstance().LoadAudioClip("Assets/Sound/gunshot.wav", "gunshot");
}

//update <- for now before updateable is implemented

int main()
{		
    AE::Core::Start(Update);

	StartApp();

	AE::Core::Run();

	AE::Core::ShutDown();
	
	return 0;
}

