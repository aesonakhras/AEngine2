#pragma once
#include <memory>
#include "TextureFormat.h"


namespace AE::Core::Graphics {
	enum TextureBindFlag : uint32_t
	{
		None = 0,
		ShaderResource = 1,
		RenderTarget = 1 << 1,
		DepthStencil = 1 << 2,
	};

	
	//TODO: Move this to graphics folder, maybe in a common folder??
	struct TextureCreateInfo {
		const void* data = nullptr;
		size_t width;
		size_t height;
		uint32_t depth = 1;	//more than one is a 3d texture
		uint32_t mipLevels = 1;
		TextureFormat format;
		TextureBindFlag bindFlags;
		bool generateMipMaps = false;
		size_t arraySize = 1;
		size_t sampleCount;
	};
}
