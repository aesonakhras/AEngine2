#include "PlayerFactory.h"

#include "Components/Player.h"
#include "Resources/ResourceManager.h"
#include "Core/Factories/StaticMeshFactory.h"

using namespace AE::App;
using namespace AE::Core;
using namespace AE::Graphics;

entt::entity PlayerFactory::Create(
	entt::registry& registry, 
	AE::Graphics::Material& material,
	Transform transform
) {
	auto playerMesh = ResourceManager::GetInstance().GetStaticMesh(std::string("Assets/Rock/Rock.obj"));

	auto player = StaticMeshFactory::Create(
		registry,
		*playerMesh.get(),
		material,
		{ 0.0f , 0.0, 15.0f },
		DirectX::XMQuaternionRotationRollPitchYawFromVector({ 0.0f, 0.0f, 0.0f, 0.0f }),
		{ 1.0f, 1.0f, 1.0f },
		nullptr,
		"Player"
	);

	registry.emplace<Player>(player, 5.0f);
	registry.emplace<Movement>(player);

	return player;
}

