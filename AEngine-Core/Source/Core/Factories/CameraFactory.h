#pragma once
#include <DirectXMath.h>
#include <entt/entt.hpp>

#include "Core/Common.h"
#include "Math/Vec3.h"

namespace AE::Core {
	class CameraFactory {
	public:
		static entt::entity Create(
			entt::registry& registry,
			float32 fov,
			float32 aspectRatio,
			float32 nearZ,
			float32 farZ,
			Vec3 position,
			Vec3 lookAt
		);
	};
}