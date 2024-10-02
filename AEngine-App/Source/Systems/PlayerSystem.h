#pragma once

#include <entt/entt.hpp>
#include <Core/Common.h>

#include "System/MultiThreading/JobSystem.h"
#include "System/MultiThreading/CommandBuffer.h"

namespace AE::App {
	class PlayerSystem {
		public:
			void Update(AE::Core::float32 deltaTime,
				entt::registry& scene,
				AE::Core::JobSystem& jobSystem,
				AE::Core::CommandBuffer& commandBuffer
			);
	};
}