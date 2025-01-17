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
	Transform transform
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
		{ 0.0, 0.0, 0.0f },
		Lookat
	);

	//auto playerMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/Rock/Rock.obj"));

	//auto player = StaticMeshFactory::Create(
	//	registry,
	//	*playerMesh.get(),
	//	material,
	//	{ 0.0f , 0.0, 15.0f },
	//	DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0.0f, 0.0f, 0.0f, 0.0f }),
	//	{ 1.0f, 1.0f, 1.0f },
	//	nullptr,
	//	"Player"
	//);

	registry.emplace<Player>(player, 5.0f);
	registry.emplace<Movement>(player);
	registry.emplace<PointLight>(player, 
		PointLight{ 
			50.0f,
			{ 0,0,1 },
			10,
			0.09f,
			0.032f
			}
		);

	return player;
}

