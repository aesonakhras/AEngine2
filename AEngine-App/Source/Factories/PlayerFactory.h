#pragma once

#include <entt/entt.hpp>

#include "Core/Components/Transform.h"
#include "Core/Components/Material.h"
#include "Core/Components/Mesh.h"

namespace AE::App {
	class PlayerFactory {
	public:
		static entt::entity Create(
			entt::registry& registry,
			AE::Core::Mesh meshName,
			AE::Graphics::Material material,
			Transform transform
		);
	};
}