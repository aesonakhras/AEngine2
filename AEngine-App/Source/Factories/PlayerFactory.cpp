#include "PlayerFactory.h"

#include "Components/Player.h"

#include "Core/Factories/StaticMeshFactory.h"

using namespace AE::App;
using namespace AE::Core;
using namespace AE::Graphics;

entt::entity PlayerFactory::Create(
	entt::registry& registry, 
	AE::Core::Mesh meshName,
	AE::Graphics::Material material,
	Transform transform
) {
	entt::entity entity = registry.create();

	registry.emplace<Mesh>(entity, meshName);
	registry.emplace<Material>(entity, material);
	registry.emplace<Transform>(entity, transform);
	registry.emplace<Player>(entity);

	return entity;
}

