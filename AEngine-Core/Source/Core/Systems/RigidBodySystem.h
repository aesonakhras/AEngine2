#pragma once

#include <entt/entt.hpp>

#include "Core/Types.h"
#include "Physics/CollisionTracker.h"

namespace AE::Physics {
	class RigidBodySystem {
	public:
		void PreUpdate(entt::registry& registry, float32 timeStep);
		void Update(entt::registry& registry, float32 timeStep);

	private:
		CollisionTracker collisionTracker;
	};
}