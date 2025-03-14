#pragma once

#include "Core/Types.h"
#include "Math/Vec3.h"

namespace AE::App {
	struct Projectile
	{
		Projectile(Vec3 direction, float32 speed) :
		Direction(direction),
		Speed(speed),
		TimeSinceSpawn(0.0f) { }

		Vec3 Direction;
		float32 Speed;
		float32 TimeSinceSpawn;
	};
}