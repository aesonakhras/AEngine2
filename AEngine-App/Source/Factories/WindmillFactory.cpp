#include <random>
#include "WindMillFactory.h"

#include "Components/WindMill.h"
#include "Resources/ResourceManager.h"
#include "Core/Factories/StaticMeshFactory.h"

#include "Physics/RigidBodyCreateInfo.h"
#include "Core/Components/RigidBody.h"

using namespace AE::App;
using namespace AE::Core;
using namespace AE::Graphics;
using namespace AE::Physics;

entt::entity WindMillFactory::Create(
	entt::registry& registry,
	AE::Graphics::Material& material,
    const Vec3& startPosition,
    float rotation,
    std::shared_ptr<ISampler> sampler
) {
    float scale = 0.5f;

     std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 6.0f);

	//create the blade static meshes with proper parenting
	auto bladeMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/Windmill/Blade.obj"));
	auto pinMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/Windmill/Pin.obj"));
	auto houseMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/Windmill/House.obj"));

    std::shared_ptr<Texture> windMillColor = ResourceManager::GetInstance().GetTexture(std::string("Assets/Windmill/BaseColor.png"), false, true);
    std::shared_ptr<Texture> windMillNormal = ResourceManager::GetInstance().GetTexture(std::string("Assets/Windmill/Normal.png"), false, true);
    std::shared_ptr<Texture> windMillMetallic = ResourceManager::GetInstance().GetTexture(std::string("Assets/Black.png"), false, true);
    std::shared_ptr<Texture> windMillRoughness = ResourceManager::GetInstance().GetTexture(std::string("Assets/Windmill/Roughness.png"), false, true);
    std::shared_ptr<Texture> brdfLUT = ResourceManager::GetInstance().GetTexture(std::string("Assets/ibl_brdf_lut.png"), false, true);

    auto windMillMaterial = ResourceManager::GetInstance().CreateMaterialInstance("DefaultMaterial");

    windMillMaterial->SetTexture("BaseColor", 0, windMillColor, sampler);
    windMillMaterial->SetTexture("Normal", 1, windMillNormal, sampler);
    windMillMaterial->SetTexture("Metallic", 2, windMillMetallic, sampler);
    windMillMaterial->SetTexture("Roughness", 3, windMillRoughness, sampler);

    windMillMaterial->SetTexture("brdf_lut", 4, brdfLUT, sampler);

	auto house = StaticMeshFactory::Create(
        registry, 
        *houseMesh.get(), 
        *windMillMaterial.get(),
        startPosition,
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0.0f, rotation, 0.0f, 0.0f }),
        { scale,scale,scale },
        entt::null,
        "House"
    );

    AE::Physics::RigidBodyCreateInfo info = {
        AE::Physics::RigidBodyShape::BOX,
        AE::Physics::BoxPhysicsShapeCreateInfo({ 6.0f, 25.0f, 6.0f}),
        AE::Physics::RigidBodyType::Static,
        startPosition,
        0.0f,
        {0,0,0}
    };

    registry.emplace<RigidBody>(
            house,
            //rigidBody
            house,
            info
        );

    auto pin = StaticMeshFactory::Create(
        registry,
        *pinMesh.get(),
        *windMillMaterial.get(),
        { 0.0f , 0.0, 0.0f },
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0.0f, 0.0f, 0.0f, 0.0f }),
        { 1.0f, 1.0f, 1.0f},
        house,
        "Pin"
    );

    auto blade1 = StaticMeshFactory::Create(
        registry, 
        *bladeMesh.get(), 
        *windMillMaterial.get(),
        { 0.0f , 0.0, 0.0f },
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0.0f, 0.0f, 0.0f, 0.0f }),
        { 1.0f, 1.0f, 1.0f },
        pin,
        "Blade1"
    );

    AE::Physics::RigidBodyCreateInfo info2 = {
        AE::Physics::RigidBodyShape::BOX,
        AE::Physics::BoxPhysicsShapeCreateInfo({ 1.0f, 5.0f, 1.0f}),
        AE::Physics::RigidBodyType::Static,
        startPosition,
        0.0f,
        {0,0,0}
    };

    registry.emplace<RigidBody>(blade1, blade1, info2);
    
    auto blade2 = StaticMeshFactory::Create(
        registry, 
        *bladeMesh.get(), 
        *windMillMaterial.get(),
        { 0.0f , 0.0, 0.0f },
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 1.5708f, 0.0f, 0.0f, 0.0f }),
        { 1.0f, 1.0f, 1.0f },
        pin,
        "Blade2"
    );

    auto blade3 = StaticMeshFactory::Create(
        registry, 
        *bladeMesh.get(), 
        *windMillMaterial.get(),
        { 0.0f , 0.0, 0.0f },
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 3.14159f, 0.0f, 0.0f, 0.0f }),
        { 1.0f, 1.0f, 1.0f },
        pin,
        "Blade3"
    );

    auto blade4 = StaticMeshFactory::Create(
        registry, 
        *bladeMesh.get(), 
        *windMillMaterial.get(),
        { 0.0f , 0.0, 0.0f },
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ -1.5708f, 0.0f, 0.0f, 0.0f }),
        { 1.0f, 1.0f, 1.0f },
        pin,
        "Blade4"
    );

    float randomWindSpeed = dist(gen);

	registry.emplace<WindMill>(house, 1.0f, blade1, blade2, blade3, blade4, pin, randomWindSpeed);
	
    //base of the mesh
	return house;
}
