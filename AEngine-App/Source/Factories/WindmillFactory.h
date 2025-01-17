#pragma once

#include <memory>

#include <entt/entt.hpp>

#include "Core/Components/Transform.h"
#include "Core/Components/Material.h"

#include "Graphics/Mesh.h"
#include "Components/Movement.h"

namespace AE::App {
	class WindMillFactory {
	public:
		static entt::entity Create(
			entt::registry& registry,
			AE::Graphics::Material& material
		);
	};
}