#pragma once

#include <entt/entt.hpp>
#include <Core/Common.h>

#include "System/MultiThreading/JobSystem.h"
#include "System/MultiThreading/CommandBuffer.h"

#include "Physics/OnCollisionInfo.h"


//forward declare
struct Transform;

namespace AE::App {
	struct Movement;

	class WindmillSystem {
	public:
		void Start(entt::registry& scene);

		void Update(
			float32 deltaTime,
			entt::registry& scene,
			AE::Core::JobSystem& jobSystem,
			AE::Core::CommandBuffer& commandBuffer
		);

		void OnCollision(const AE::Physics::OnCollisionInfo& collisionInfo, entt::entity);

	private:
		entt::registry* scene;

		float fireRate = 6.0f;
	};
}