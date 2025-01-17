#pragma once

#include "Core/Types.h"
#include "Math/Vec3.h"

namespace AE::App {
	struct Projectile
	{
		Vec3 Direction;
		float32 Speed;
	};
}