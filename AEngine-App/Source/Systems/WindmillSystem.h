#pragma once

#include <entt/entt.hpp>
#include <Core/Common.h>

#include "System/MultiThreading/JobSystem.h"
#include "System/MultiThreading/CommandBuffer.h"

//forward declare
struct Transform;

namespace AE::App {
	struct Movement;

	class WindmillSystem {
	public:
		void Update(
			float32 deltaTime,
			entt::registry& scene,
			AE::Core::JobSystem& jobSystem,
			AE::Core::CommandBuffer& commandBuffer
		);
	};
}