#pragma once
#include <vector>
#include <memory>

#include "Core/Types.h"
#include "Math/Vec3.h"

namespace AE::Physics {
	struct AABB
	{
		Vec3 min;
		Vec3 max;
	};
}