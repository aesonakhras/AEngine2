#include "StaticMeshFactory.h"

#include "FileManagment/MeshData.h"
#include "FileManagment/FileImporter.h"

#include "Graphics/GraphicsManager.h"
#include "Graphics/CommonVerticies.h"

#include "Core/Systems/SystemLocator.h"
#include "Core/Systems/TransformSystem.h"

using namespace AE::Core;
using namespace AE::Graphics;

entt::entity StaticMeshFactory::Create(
	entt::registry& registry,
	AE::Graphics::Mesh meshName,
	AE::Graphics::Material& material,
	Vec3 pos,
	DirectX::XMVECTOR rot,
	Vec3 scale,
	entt::entity parent,
	std::string name
) {
	entt::entity entity = registry.create();

	registry.emplace<Mesh>(entity, meshName);
	registry.emplace<Material>(entity, material);
	registry.emplace<Transform>(entity, entity, pos, rot, scale, parent, name);

	//add it to the world
	auto& transform = registry.get<Transform>(entity);

	auto transforSystem = SystemLocator::Get<TransformSystem>();

	transforSystem->AddToWorld(entity, parent);

	return entity;
}
