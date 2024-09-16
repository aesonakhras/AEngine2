#pragma once
#include <memory>

namespace AE::Graphics {
	enum TextureBindFlag : uint32_t
	{
		None = 0,
		ShaderResource = 1,
		RenderTarget = 1 << 1,
		DepthStencil = 1 << 2,
	};
}