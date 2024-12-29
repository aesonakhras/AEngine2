#pragma once
#include <entt/entt.hpp>

#include <string>
#include <memory>

#include "Core/Components/Transform.h"
#include "Core/Components/Material.h"
#include "Graphics/Mesh.h"

namespace AE::Core {	
	class StaticMeshFactory {
		public:
			static entt::entity Create(
				entt::registry& registry,
				AE::Graphics::Mesh meshName,
				AE::Graphics::Material material,
				Transform transform
			);
	};
}