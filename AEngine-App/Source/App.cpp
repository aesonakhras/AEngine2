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
#include "Core/Factories/SkyboxFactory.h"

#include "FileManagment/MeshData.h"
#include "FileManagment/FileImporter.h"

#include "System/Audio/AudioManager.h"
#include "System/Input/InputManager.h"

#include "Core/Factories/CameraFactory.h"
#include "Core/Factories/PointLightFactory.h"

#include "Systems/PlayerSystem.h"
#include "Factories/PlayerFactory.h"

#include "Systems/WindmillSystem.h"
#include "Systems/ProjectileSystem.h"
#include "Factories/WindmillFactory.h"

#include "Resources/ResourceManager.h"

#include "Physics/RigidBodyCreateInfo.h"
#include "Core/Components/RigidBody.h"

#include <iostream>

using namespace AE::Core;
using namespace AE::Graphics;
using namespace AE::App;
using namespace AE::Physics;

//#define SCREEN_WIDTH 1280
//#define SCREEN_HEIGHT 720

entt::entity skybox;

AE::App::PlayerSystem playerSystem;

AE::App::WindmillSystem windmillSystem;

AE::App::ProjectileSystem projectileSystem;

std::shared_ptr<AE::Graphics::ISampler> sampler;

std::shared_ptr<AE::Graphics::ISampler> depthSampler;

std::vector<AE::Graphics::UniformDescriptionElement> standardUniformDescription = {
    { "MVP", sizeof(DirectX::XMMATRIX) },
    { "Model", sizeof(DirectX::XMMATRIX) },
    { "LightSpaceMatrix", sizeof(DirectX::XMMATRIX) },
    { "viewPos", sizeof(DirectX::XMVECTOR) },
    { "DirLight", sizeof(DirectX::XMVECTOR) }
};

//set up player
void setupPlayer(AE::Core::SceneManager& sceneManager) {
    //textures
    GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

    std::shared_ptr<Texture> baseColor = ResourceManager::GetInstance().GetTexture(std::string("Assets/Rock/basecolor.png"), false, true);
    std::shared_ptr<Texture> normal = ResourceManager::GetInstance().GetTexture(std::string("Assets/Rock/normal.png"), false, true);
    sampler = graphicsManager.CreateSampler(false);

    //TODO: Now this is epic
    depthSampler = graphicsManager.CreateSampler(true);

    depthSampler->Bind(10);

    //std::shared_ptr<Mesh> PlayerMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/Rock/rock.obj"));

    //std::vector<AE::Graphics::UniformDescriptionElement> PlayeruniformDescription = {
    //    { "MVP", sizeof(DirectX::XMMATRIX) },
    //    { "Model", sizeof(DirectX::XMMATRIX) },
    //    { "LightSpaceMatrix", sizeof(DirectX::XMMATRIX) },
    //    { "viewPos", sizeof(DirectX::XMVECTOR) },
    //    { "DirLight", sizeof(DirectX::XMVECTOR) }
    //};

    auto playerMaterial = ResourceManager::GetInstance().LoadMaterial("Assets/shaders.shader", "Assets/shaders.shader", "DefaultMaterial", standardUniformDescription);

    playerMaterial->SetTexture("diffuse1", 0, baseColor, sampler);
    playerMaterial->SetTexture("diffuse2", 1, normal, sampler);


    //NOTE: THIS ONE IS NOT USED, maerked for deletion later
    Transform playerStart = {
        {},
        { 0 , 0.0f, 15.0f},
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 1.5f, 0.0f, 0.0f, 0.0f }),
        {1.0f, 1.0f, 1.0f},
        entt::null,
        "Player"
    };

    auto player = PlayerFactory::Create(sceneManager.Registry, *playerMaterial.get(), playerStart, sampler);


    auto& theRegistry = sceneManager.Registry;

    //add a light for testing purposes
    //entt::entity testLight = theRegistry.create();

    //PointLightFactory::AddToEntity(
    //    theRegistry,
    //    testLight,
    //    { 0.0f , 1.0f, 0.0f },
    //    entt::null,
    //    5.0f,
    //    {1,0,0},
    //    10,
    //    0.09f,
    //    0.032f
    //);
}

void SetupBox(AE::Core::SceneManager& sceneManager) {
    GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

    std::shared_ptr<Texture> boxColor = ResourceManager::GetInstance().GetTexture(std::string("Assets/Box/Box_Base_Color.png"), false, true);
    std::shared_ptr<Texture> boxNormal = ResourceManager::GetInstance().GetTexture(std::string("Assets/Box/Box_Normal.png"), false, true);
    std::shared_ptr<Texture> boxMetallic = ResourceManager::GetInstance().GetTexture(std::string("Assets/Box/Box_Metallic.png"), false, true);
    std::shared_ptr<Texture> boxRoughness = ResourceManager::GetInstance().GetTexture(std::string("Assets/Box/Box_Roughness.png"), false, true);

    std::shared_ptr<Mesh> cargoBoxMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/Box/CargoBox.obj"));

    std::shared_ptr<Texture> brdfLUT = ResourceManager::GetInstance().GetTexture(std::string("Assets/ibl_brdf_lut.png"), false, false);

    auto boxMaterial = ResourceManager::GetInstance().CreateMaterialInstance("DefaultMaterial");

    boxMaterial->SetTexture("BaseColor", 0, boxColor, sampler);
    boxMaterial->SetTexture("Normal", 1, boxNormal, sampler);
    boxMaterial->SetTexture("Metallic", 2, boxMetallic, sampler);
    boxMaterial->SetTexture("Roughness", 3, boxRoughness, sampler);

    boxMaterial->SetTexture("brdf_lut", 4, brdfLUT, sampler);
}

void InitProjectileMaterial(AE::Core::SceneManager& sceneManager) {
    GraphicsManager& graphicsManager = GraphicsManager::GetInstance();

    //temporal dependency on sampler

    //std::vector<AE::Graphics::UniformDescriptionElement> projectileUniformDescription = {
    //{ "MVP", sizeof(DirectX::XMMATRIX) },
    //{ "Model", sizeof(DirectX::XMMATRIX) },
    //{ "LightSpaceMatrix", sizeof(DirectX::XMMATRIX) },
    //{ "viewPos", sizeof(DirectX::XMVECTOR) },
    //{ "DirLight", sizeof(DirectX::XMVECTOR) }
    //};

    auto projectileMaterial = ResourceManager::GetInstance().LoadMaterial("Assets/shaders.shader", "Assets/shaders.shader", "ProjectileMaterial", standardUniformDescription);



    std::shared_ptr<AE::Graphics::Texture> projectileColor = ResourceManager::GetInstance().GetTexture(std::string("Assets/pbrTest/basecolor.png"), false, true);
    std::shared_ptr<AE::Graphics::Texture> projectileNormal = ResourceManager::GetInstance().GetTexture(std::string("Assets/pbrTest/normal.png"), false, true);
    std::shared_ptr<AE::Graphics::Texture> projectileMetallic = ResourceManager::GetInstance().GetTexture(std::string("Assets/pbrTest/metallic.png"), false, true);
    std::shared_ptr<AE::Graphics::Texture> projectileRoughness = ResourceManager::GetInstance().GetTexture(std::string("Assets/pbrTest/roughness.png"), false, true);
    std::shared_ptr<AE::Graphics::Texture> brdfLUT = ResourceManager::GetInstance().GetTexture(std::string("Assets/ibl_brdf_lut.png"), false, true);

    projectileMaterial->SetTexture("BaseColor", 0, projectileColor, sampler);
    projectileMaterial->SetTexture("Normal", 1, projectileNormal, sampler);
    projectileMaterial->SetTexture("Metallic", 2, projectileMetallic, sampler);
    projectileMaterial->SetTexture("Roughness", 3, projectileRoughness, sampler);

    projectileMaterial->SetTexture("brdf_lut", 4, brdfLUT, sampler);
}

//set up environment
void setupEnvironment(AE::Core::SceneManager& sceneManager) {
    GraphicsManager& graphicsManager = GraphicsManager::GetInstance();
    
    if (sampler == nullptr) {
        sampler = graphicsManager.CreateSampler(false);
    }
    
    SetupBox(sceneManager);
    InitProjectileMaterial(sceneManager);

    std::shared_ptr<Texture> baseColor = ResourceManager::GetInstance().GetTexture(std::string("Assets/ForestGround/Forest_Ground_Diffuse.png"), false, true);
    std::shared_ptr<Texture> normal = ResourceManager::GetInstance().GetTexture(std::string("Assets/ForestGround/Forest_Ground_Normal2.png"), false, true);
    std::shared_ptr<Texture> Metallic = ResourceManager::GetInstance().GetTexture(std::string("Assets/Black.png"), false, true);
    std::shared_ptr<Texture> Roughness = ResourceManager::GetInstance().GetTexture(std::string("Assets/ForestGround/Forest_Ground_Roughness.png"), false, true);

    std::shared_ptr<Mesh> planeMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/plane.obj"));

    std::shared_ptr<Mesh> sphereMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/Sphere.obj"));

    std::shared_ptr<Mesh> PlayerMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/Rock/rock.obj"));

    std::shared_ptr<Texture> brdfLUT = ResourceManager::GetInstance().GetTexture(std::string("Assets/ibl_brdf_lut.png"), false, false);

    auto planeMaterial = ResourceManager::GetInstance().LoadMaterial(
        "Assets/shaders_tiled.shader",
        "Assets/shaders_tiled.shader",
        "tiledShader",
        standardUniformDescription
    );


    if (sampler == nullptr) {
        sampler = graphicsManager.CreateSampler(false);
    }

    planeMaterial->SetTexture("BaseColor", 0, baseColor, sampler);
    planeMaterial->SetTexture("Normal", 1, normal, sampler);
    planeMaterial->SetTexture("Metallic", 2, Metallic, sampler);
    planeMaterial->SetTexture("Roughness", 3, Roughness, sampler);


    planeMaterial->SetTexture("brdf_lut", 4, brdfLUT, sampler);

    Vec3 groundPos = { 0.0f, -1.0f, 0.0f };

    auto sphereMaterial = ResourceManager::GetInstance().GetSharedMaterial("ProjectileMaterial");

    float sphereScale = 6.0f;
    float sphereY = 10.0f;

    auto sphere = StaticMeshFactory::Create(
        sceneManager.Registry,
        *sphereMesh.get(),
        *sphereMaterial.get(),
        { 0.0f, sphereY, 0.0f},
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0.0f, 0.0f, 0.0f, 0.0f }),
        { sphereScale, sphereScale, sphereScale },
        entt::null,
        "Sphere"
    );

    auto ground = StaticMeshFactory::Create(
        sceneManager.Registry, 
        *planeMesh.get(),
        *planeMaterial.get(), 
        groundPos,
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0.0f, 0.0f, 0.0f, 0.0f }),
        { 45.0f, 45.0f, 45.0f },
        entt::null,
        "Sphere"
    );


    AE::Physics::RigidBodyCreateInfo info = {
            AE::Physics::RigidBodyShape::BOX,
            AE::Physics::BoxPhysicsShapeCreateInfo({50.0f,0.01f,50.0f}),
            AE::Physics::RigidBodyType::Static,
            groundPos,
            0.0f,
            {0,0,0},
            0.5f,
            0.5f
    };

    AE::Physics::RigidBodyCreateInfo sphereInfo = {
        AE::Physics::RigidBodyShape::SPHERE,
        AE::Physics::SpherePhysicsShapeCreateInfo({sphereScale}),
        AE::Physics::RigidBodyType::Static,
        { 0.0f, sphereY, 0.0f },
        0.0f,
        {0,0,0},
        5.0f,
        1.0f
    };

   sceneManager.Registry.emplace < AE::Physics::RigidBody>(sphere, sphere, sphereInfo);

    sceneManager.Registry.emplace<AE::Physics::RigidBody>(
        ground, 
        //rigidBody
        ground,
        info
        );

    float extreme = 30.0f;
    float yOffset = 13.5f;
    float lightYOffset = 0.0f;
    float lightOtherOffset = 5.0f;

    //front
    auto windMill = WindMillFactory::Create(
        sceneManager.Registry,
        *planeMaterial.get(),
        { extreme, yOffset, 0.0f},
        3.14159f,
        sampler
    );

    entt::entity windMill1Light = sceneManager.Registry.create();

    PointLightFactory::AddToEntity(
        sceneManager.Registry,
        windMill1Light,
        { extreme - lightOtherOffset , yOffset + lightYOffset, 0.0f },
        entt::null,
        5.0f,
        { 1,0,0 },
        40,
        0.09f,
        0.032f
    );

    
    auto windMill2 = WindMillFactory::Create(
        sceneManager.Registry,
        *planeMaterial.get(),
        { 0.0f, yOffset, extreme },
        1.5708f,
        sampler
    );

    entt::entity windMill2Light = sceneManager.Registry.create();

    PointLightFactory::AddToEntity(
        sceneManager.Registry,
        windMill2Light,
        { 0.0f, yOffset, extreme - lightOtherOffset },
        entt::null,
        5.0f,
        { 0,1,0 },
        40,
        0.09f,
        0.032f
    );

    //left
    auto windMill3 = WindMillFactory::Create(
        sceneManager.Registry,
        *planeMaterial.get(),
        { -extreme , yOffset, 0.0f },
        0.0f,
        sampler
    );

    entt::entity windMill3Light = sceneManager.Registry.create();

    PointLightFactory::AddToEntity(
        sceneManager.Registry,
        windMill3Light,
        { -extreme + lightOtherOffset, yOffset, 0.0f },
        entt::null,
        5.0f,
        { 0,0,1 },
        40,
        0.09f,
        0.032f
    );

    auto windMill4 = WindMillFactory::Create(
        sceneManager.Registry,
        *planeMaterial.get(),
        { 0.0f, yOffset, -extreme },
        4.71f,
        sampler
    );

    entt::entity windMill4Light = sceneManager.Registry.create();

    PointLightFactory::AddToEntity(
        sceneManager.Registry,
        windMill4Light,
        { 0.0f, yOffset, -extreme + lightOtherOffset },
        entt::null,
        5.0f,
        { 1,1,0 },
        40,
        0.09f,
        0.032f
    );

    //add the skybox
    skybox = AE::Core::SkyboxFactory::Create(
        sceneManager.Registry,
        std::string("Assets/SkyBox/final_v.ktx2"),
        std::string("Assets/SkyBox/final_r.ktx2"),
        std::string("Assets/SkyBox/final_i.ktx2")
        //std::string("Assets/SkyBox/alley_r.ktx2"),
        //std::string("Assets/SkyBox/alley_r.ktx2"),
        //std::string("Assets/SkyBox/alley_i.ktx2")
    );

    //skybox = AE::Core::SkyboxFactory::Create(sceneManager.Registry, std::string("Assets/SkyBox/"));

    windmillSystem.Start(sceneManager.Registry);

    
}


void SetupScene() {
    //get scene ref
    AE::Core::SceneManager& sceneManager = AE::Core::SceneManager::GetInstance();

    setupPlayer(sceneManager);

    setupEnvironment(sceneManager);
}

void Update(float32 deltaTime,
    AE::Core::JobSystem& jobSystem,
    AE::Core::CommandBuffer& commandBuffer
) {
    AE::Core::SceneManager& sceneManager = AE::Core::SceneManager::GetInstance();

    windmillSystem.Update(deltaTime, sceneManager.Registry, jobSystem, commandBuffer);
    playerSystem.Update(deltaTime, sceneManager.Registry, jobSystem, commandBuffer);
    projectileSystem.Update(deltaTime, sceneManager.Registry, jobSystem, commandBuffer);
}

//startup
void StartApp() {
    SetupScene();

    //TODO: convert to managed resource at some point
    AE::System::AudioManager::GetInstance().LoadAudioClip("Assets/Sound/gunshot.wav", "gunshot");
    AE::System::AudioManager::GetInstance().LoadAudioClip("Assets/Sound/gunshot_2.wav", "gunshot2");
    AE::System::AudioManager::GetInstance().LoadAudioClip("Assets/Sound/bg_Music.wav", "bgMusic");
    AE::System::AudioManager::GetInstance().LoadAudioClip("Assets/Sound/hit.wav", "hit");

    AE::System::AudioManager::GetInstance().PlayAudio("bgMusic");

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

