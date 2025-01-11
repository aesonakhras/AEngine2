#pragma once

#include <entt/entt.hpp>

namespace AE::Core {
	class RenderSystem {
		public:
			void Render();
			void MaybeRenderSkyBox(DirectX::XMMATRIX view, DirectX::XMMATRIX projection);
	};
}