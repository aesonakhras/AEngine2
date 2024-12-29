#include <memory>
#include <vector>

#include <entt/entt.hpp>

#include "Core/Core.h"

//TODO: determine if this should just be in a common.h or core.h or something
#include "Graphics/GraphicsManager.h"

#include "Core/Components/Material.h"
#include "Graphics/CommonVerticies.h"

#include "Core/Factories/StaticMeshFactory.h"

#include "Core/Scene/SceneManager.h"

#include "FileManagment/MeshData.h"
#include "FileManagment/FileImporter.h"

#include "System/Audio/AudioManager.h"
#include "System/Input/InputManager.h"

#include "Core/Factories/CameraFactory.h"

#include "Systems/PlayerSystem.h"
#include "Factories/PlayerFactory.h"

#include "Resources/ResourceManager.h"

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
Vec3 Lookat = { 0.0f, 0.0f, -1.0f};

entt::entity camera;

AE::App::PlayerSystem playerSystem;

std::shared_ptr<AE::Graphics::ISampler> sampler;

//set up player
void setupPlayer(AE::Core::SceneManager& sceneManager) {
    //textures
    GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

    std::shared_ptr<Texture> baseColor = ResourceManager::GetInstance().GetTexture(std::string("Assets/Rock/basecolor.png"));
    std::shared_ptr<Texture> normal = ResourceManager::GetInstance().GetTexture(std::string("Assets/Rock/normal.png"));

    //std::shared_ptr<AE::Graphics::ISampler> PlayerSampler = graphicsManager.CreateSampler();

    sampler = graphicsManager.CreateSampler();

    std::shared_ptr<Mesh> PlayerMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/Rock/rock.obj"));

    std::shared_ptr<Material> playerMaterial = ResourceManager::GetInstance().GetMaterial("Assets/shaders.shader", "Assets/shaders.shader", "RockMaterial");

    playerMaterial->SetTexture("diffuse1", 0, baseColor, sampler);
    playerMaterial->SetTexture("diffuse2", 1, normal, sampler);

    Transform playerStart = {
        { 0 , 0.0f, 15.0f},
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 1.5f, 0.0f, 0.0f, 0.0f }),
        {1.0f, 1.0f, 1.0f}
    };

    auto player = PlayerFactory::Create(sceneManager.Registry, *PlayerMesh.get(), *playerMaterial.get(), playerStart);

}

//set up environment
void setupEnvironment(AE::Core::SceneManager& sceneManager) {
    GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

    std::shared_ptr<Texture> normal = ResourceManager::GetInstance().GetTexture(std::string("Assets/Rock/normal.png"));

    std::shared_ptr<Mesh> planeMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/plane.obj"));

    std::shared_ptr<Material> planeMaterial = ResourceManager::GetInstance().GetMaterial("Assets/shaders.shader", "Assets/shaders.shader", "planeMaterial");

    Transform planeStart = {
        { 0.0f , -4.0f, 15.0f},
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0.0f, 0.0f, 0.0f, 0.0f }),
        {5.0f, 5.0f, 5.0f}
    };

    if (sampler == nullptr) {
        sampler = graphicsManager.CreateSampler();
    }

    planeMaterial->SetTexture("diffuse1", 0, normal, sampler);
    planeMaterial->SetTexture("diffuse2", 0, normal, sampler);

    auto ground = StaticMeshFactory::Create(sceneManager.Registry, *planeMesh.get(), *planeMaterial.get(), planeStart);
}

//set up enemies



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

    setupPlayer(sceneManager);

    setupEnvironment(sceneManager);
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

    //TODO: convert to managed resource at some point
    //AE::System::AudioManager::GetInstance().LoadAudioClip("Assets/Sound/gunshot.wav", "gunshot");

    AE::Core::SceneManager& sceneManager = AE::Core::SceneManager::GetInstance();
    playerSystem.Start(sceneManager.Registry);
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

