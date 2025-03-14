#pragma once

#include <memory>
//#include "Graphics/GraphicsManager.h"

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
			AE::Graphics::Material& material,
			const Vec3& startPosition,
			float rotation,
			std::shared_ptr<AE::Graphics::ISampler> sampler
		);
	};
}