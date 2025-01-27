#pragma once
#include "entt/entt.hpp"

#include "Math/Vec3.h"
namespace AE::Physics {
	struct OnCollisionInfo
	{
		Vec3 Position;
		Vec3 Normal;
		entt::entity Other;
	};
}