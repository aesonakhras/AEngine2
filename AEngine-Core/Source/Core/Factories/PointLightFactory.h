#pragma once
#include "entt/entt.hpp"

#include "Core/Types.h"
#include "Math/Vec3.h"

#include "Core/Components/Transform.h"

namespace AE::Graphics {
	class PointLightFactory {
		public:
			static void AddToEntity(
				entt::registry& registry,
				entt::entity entity,
				Vec3 position,
				Transform* parent,
				float intensity,
				Vec3 color,
				float radius,
				float linear,
				float quad
			);
		private:
	};
}