#pragma once
#include "Core/Types.h"
#include "Math/Vec3.h"

namespace AE::Graphics {
	struct PointLight
	{
		float Intensity;

		Vec3 Color;
		float Radius;

		//attenuation
		float Linear;
		float Quad;
	};
}