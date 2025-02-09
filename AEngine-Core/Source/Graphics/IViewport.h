#pragma once

#include "Core/Types.h"

namespace AE::Graphics {
	struct ViewPortCreateInfo {
		float32 x;
		float32 y;
		float32 width;
		float32 height;
		float32 minDepth;
		float32 maxDepth;
	};

	class IViewport {
		public:
			virtual void Bind() = 0;
			virtual ~IViewport() = default;
	};
}