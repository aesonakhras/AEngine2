#include "WindMillFactory.h"

#include "Components/WindMill.h"
#include "Resources/ResourceManager.h"
#include "Core/Factories/StaticMeshFactory.h"

using namespace AE::App;
using namespace AE::Core;
using namespace AE::Graphics;

entt::entity WindMillFactory::Create(
	entt::registry& registry,
	AE::Graphics::Material& material,
	Transform transform
) {

	//create the blade static meshes with proper parenting
	auto bladeMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/Windmill/Blade.obj"));
	auto pinMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/Windmill/Pin.obj"));
	auto houseMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/Windmill/House.obj"));


	auto house = StaticMeshFactory::Create(
        registry, 
        *houseMesh.get(), 
        material, 
        { 0.0f , 0.0, 6.0f },
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0.0f, 1.5708f, 0.0f, 0.0f }),
        { 0.2f, 0.2f, 0.2f},
        nullptr,
        "House"
    );

    auto& houseTransform = registry.get<Transform>(house);

    auto pin = StaticMeshFactory::Create(
        registry,
        *pinMesh.get(),
        material,
        { 0.0f , 0.0, 0.0f },
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0.0f, 0.0f, 0.0f, 0.0f }),
        { 1.0f, 1.0f, 1.0f},
        & houseTransform,
        //nullptr,
        "Pin"
    );

    //wow
    auto& pinTransform = registry.get<Transform>(pin);

    auto blade1 = StaticMeshFactory::Create(
        registry, 
        *bladeMesh.get(), 
        material, 
        { 0.0f , 0.0, 0.0f },
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0.0f, 0.0f, 0.0f, 0.0f }),
        { 1.0f, 1.0f, 1.0f },
        &pinTransform,
        "Blade1"
    );
    
    auto blade2 = StaticMeshFactory::Create(
        registry, 
        *bladeMesh.get(), 
        material, 
        { 0.0f , 0.0, 0.0f },
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 1.5708f, 0.0f, 0.0f, 0.0f }),
        { 1.0f, 1.0f, 1.0f },
        &pinTransform,
        "Blade2"
    );

    auto blade3 = StaticMeshFactory::Create(
        registry, 
        *bladeMesh.get(), 
        material, 
        { 0.0f , 0.0, 0.0f },
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ 3.14159f, 0.0f, 0.0f, 0.0f }),
        { 1.0f, 1.0f, 1.0f },
        &pinTransform,
        "Blade3"
    );

    auto blade4 = StaticMeshFactory::Create(
        registry, 
        *bladeMesh.get(), 
        material, 
        { 0.0f , 0.0, 0.0f },
        DirectX::XMQuaternionRotationRollPitchYawFromVector({ -1.5708f, 0.0f, 0.0f, 0.0f }),
        { 1.0f, 1.0f, 1.0f },
        &pinTransform,
        "Blade4"
    );

	registry.emplace<WindMill>(pin, 1.0f);
	
    //base of the mesh
	return house;
}
