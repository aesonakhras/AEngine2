#include "PlayerFactory.h"

#include "Components/Player.h"
#include "Resources/ResourceManager.h"
#include "Core/Factories/StaticMeshFactory.h"
#include "Core/Factories/CameraFactory.h"
#include "Core/Components/PointLight.h"

using namespace AE::App;
using namespace AE::Core;
using namespace AE::Graphics;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

entt::entity PlayerFactory::Create(
	entt::registry& registry, 
	AE::Graphics::Material& material,
	Transform transform,
	std::shared_ptr<AE::Graphics::ISampler> sampler
) {
	float fov = 1.0472f;
	float aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	float nearZ = 0.01f;
	float farZ = 1000.0f;
	Vec3 Lookat = { 0.0f, 0.0f, 1.0f };

	//entt::entity player = registry.create();
	auto player = AE::Core::CameraFactory::Create(
		registry,
		fov,
		aspectRatio,
		nearZ,
		farZ,
		false,
		10,
		{ 0.0, 0.0, 0.0f },
		Lookat,
		true
	);


	std::shared_ptr<Texture> akColor = ResourceManager::GetInstance().GetTexture(std::string("Assets/AK/AK47_BaseColor.png"), false, false);
	std::shared_ptr<Texture> akNormal = ResourceManager::GetInstance().GetTexture(std::string("Assets/AK/AK47_Normal.png"), false, false);
	std::shared_ptr<Texture> akMetallic = ResourceManager::GetInstance().GetTexture(std::string("Assets/AK/AK47_Metallic.png"), false, false);
	std::shared_ptr<Texture> akRoughness = ResourceManager::GetInstance().GetTexture(std::string("Assets/AK/AK47_Roughness.png"), false, false);

	std::shared_ptr<Mesh> akMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/AK/AK47.obj"));

	std::shared_ptr<Texture> brdfLUT = ResourceManager::GetInstance().GetTexture(std::string("Assets/ibl_brdf_lut.png"), false, false);

	auto akMaterial = ResourceManager::GetInstance().CreateMaterialInstance("DefaultMaterial");

	akMaterial->SetTexture("BaseColor", 0, akColor, sampler);
	akMaterial->SetTexture("Normal", 1, akNormal, sampler);
	akMaterial->SetTexture("Metallic", 2, akMetallic, sampler);
	akMaterial->SetTexture("Roughness", 3, akRoughness, sampler);

	akMaterial->SetTexture("brdf_lut", 4, brdfLUT, sampler);
	auto playerGun = StaticMeshFactory::Create(
		registry,
		*akMesh.get(),
		*akMaterial.get(),
		{ 1.0f , -0.5, 1.5f },
		DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0.0f, 3.14f / 2.0f, 0.0f, 0.0f }),
		{ 3.5f, 3.5f, 3.5f },
		player,
		"PlayerMesh"
	);

	registry.emplace<Player>(player, 5.0f);
	registry.emplace<Movement>(player);
	//registry.emplace<PointLight>(player, 
	//	PointLight{ 
	//		50.0f,
	//		{ 0,0,1 },
	//		10,
	//		0.09f,
	//		0.032f
	//		}
	//	);

	return player;
}

