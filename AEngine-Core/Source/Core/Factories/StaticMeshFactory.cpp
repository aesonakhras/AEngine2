#include "StaticMeshFactory.h"

#include "FileManagment/MeshData.h"
#include "FileManagment/FileImporter.h"

#include "Graphics/GraphicsManager.h"
#include "Graphics/CommonVerticies.h"

using namespace AE::Core;
using namespace AE::Graphics;

entt::entity StaticMeshFactory::CreateStaticMesh(entt::registry& registry, Mesh meshName, Material material, Transform transform) {
	entt::entity entity = registry.create();

	

	registry.emplace<Mesh>(entity, meshName);
	registry.emplace<Material>(entity, material);
	registry.emplace<Transform>(entity, transform);

	return entity;
}
