#include "PlayerFactory.h"

#include "Components/Player.h"

using namespace AE::App;
using namespace AE::Core;
using namespace AE::Graphics;

entt::entity PlayerFactory::Create(
	entt::registry& registry, 
	Mesh meshName,
	AE::Graphics::Material& material,
	Transform transform
) {
	entt::entity entity = registry.create();

	registry.emplace<Mesh>(entity, meshName);
	registry.emplace<Material>(entity, material);
	registry.emplace<Transform>(entity, transform);
	registry.emplace<Player>(entity);

	return entity;
}

